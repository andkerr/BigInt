#ifndef BIGINT_H
#define BIGINT

// A class to represent arbitrary-precision integers.
// by Andrew Kerr <kerrand@protonmail.com>, January 2022


#include <iostream>
#include <vector>
#include <string>


class BigInt {
    private:
        // BigInts are stored as an underlying vector of
        // integers with values between 0 and 9 inclusive.
        // The digits are stored in least-significant digit order,
        // i.e. the first element in the digits vector represents
        // the ones place, in hopes that this will provide good
        // performance when adding more digits with a call to
        // push_back(). I'm not sure if there is a better way to
        // do this, but have considered storing digits in a deque.
        std::vector<char> digits;

    void carry(const int index);

    public:
        BigInt(const std::string &val);

        BigInt operator+(const BigInt &rhs);

        friend std::ostream& operator<<(std::ostream &os,
                                        const BigInt &val);

};

#endif
