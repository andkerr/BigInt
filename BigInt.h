#ifndef BIGINT_H
#define BIGINT_H

// A class to represent arbitrary-precision integers.
// by Andrew Kerr <kerrand@protonmail.com>, January 2022


#include <iostream>
#include <vector>
#include <string>


class BigInt {
    public:
        BigInt(); // default ctor
        BigInt(const std::string &val); // ctor
        BigInt& operator=(const std::string &val); // assignment from string

        bool is_negative() const;

        // arithmetic-assignment operators
        BigInt& operator+=(const BigInt &rhs);
        BigInt& operator-=(const BigInt &rhs);

        // arithmetic operators
        BigInt operator+(const BigInt &rhs) const;
        BigInt operator-(const BigInt &rhs) const;

        // unary operators
        BigInt operator+();
        BigInt operator-();
        BigInt& operator++();
        BigInt& operator--();
        BigInt operator++(int);
        BigInt operator--(int);

        std::string to_string() const;
        friend std::ostream& operator<<(std::ostream &os,
                                        const BigInt &val);

    private:
        // BigInts are stored as an underlying vector of
        // integers whose values are between 0 and 9 inclusive.
        // The digits are stored in least-significant digit order,
        // i.e. the first element in the digits vector represents
        // the ones place, in hopes that this will provide good
        // performance when adding more digits with calls to
        // push_back(). I'm not sure if there is a better way to
        // do this, but have considered storing digits in a deque.
        std::vector<int> digits;

        bool negative;

        void carry(const int index);

};

#endif // BIGINT_H
