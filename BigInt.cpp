#include <cassert>
#include <exception>
#include "BigInt.h"

BigInt::BigInt()
    : digits({0}), negative(false) { }

BigInt::BigInt(const std::string &val) {
    if (val.empty()) {
        throw std::invalid_argument(
            "BigInt cannot be initialized from an empty string."
        );
    }

    if (val.front() == '-') {
        if (val.size() == 1) {
            throw std::invalid_argument(
                "Initializer string must contain a decimal value."
            );
        }
        negative = true;
    }

    for (auto it = val.rbegin(); it != val.rend(); ++it) {
        if (*it == '-' && next(it) == val.rend()) {
            continue;
        }

        if (*it < '0' || *it > '9') {
            throw std::invalid_argument("Bad initializer digit.");
        }

        digits.push_back(*it - '0');
    }
}

bool BigInt::is_negative() const {
    return negative;
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

BigInt& BigInt::operator+=(const BigInt &rhs) {
    // define in terms of the overloaded addition operator
    return *this = *this + rhs;
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

BigInt BigInt::operator-(const BigInt &rhs) {
    assert(false);
}

BigInt BigInt::operator+() {
    return *this;
}

BigInt BigInt::operator-() {
    negative = negative ? false : true;
    return *this;
}

std::ostream& operator<<(std::ostream &os, const BigInt &val)
{
    if (val.is_negative()) {
        os << '-';
    }

    for (auto it = val.digits.rbegin(); it != val.digits.rend(); ++it)
    {
        os << *it;
    }

    return os;
}
