#include <cassert>
#include "BigInt.h"

BigInt::BigInt()
    : digits({0}), negative(false) { }

BigInt::BigInt(const std::string &val) {
    for (auto it = val.rbegin(); it != val.rend(); ++it) {
        digits.push_back(*it - '0');
    }
}

void BigInt::carry(const int index) {
    assert(digits[index] > 9 && digits[index] < 20);

    if (index == int(digits.size()) - 1) {
        digits.push_back(1);
        digits[index] %= 10;
    }
    else {
        digits[index + 1] += 1;
        digits[index] %= 10;

        if (digits[index + 1] > 9) {
            carry(index+1);
        }
    }
}

void BigInt::operator+=(const BigInt &rhs) {
    // define in terms of the overloaded addition operator
    *this = *this + rhs;
}

BigInt BigInt::operator+(const BigInt &rhs) {
    BigInt result = *this;

    for (size_t i = 0; i < rhs.digits.size(); ++i) {
        int digit = rhs.digits[i];

        assert(i <= result.digits.size());
        if (i == result.digits.size()) {
            result.digits.push_back(digit);
        }
        else {
            result.digits[i] += digit;
            if (result.digits[i] > 9) {
                result.carry(i);
            }
        }
    }

    return result;
}

std::ostream& operator<<(std::ostream &os, const BigInt &val)
{
    for (auto it = val.digits.rbegin(); it != val.digits.rend(); ++it)
    {
        os << *it;
    }

    return os;
}
