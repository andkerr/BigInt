#include <algorithm>
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

// assign to a BigInt from a string representation of an integer
//
// * is this a good idea? I am implementing this with convenience
// in mind, for example, to reassign to a previously-created BigInt a:
// a = "12345"; (instead of/in addition to)
// a = BigInt("12345");
//
// Is this confusing more than it is helpful?
BigInt& BigInt::operator=(const std::string &val) {
    return *this = BigInt(val);
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

BigInt BigInt::operator+(const BigInt &rhs) const {
    BigInt result = *this;

    int i = 0;
    int carry = 0;
    while (i < rhs.digits.size() || i < result.digits.size()) {
        if (i < rhs.digits.size() && i < result.digits.size()) {
            int partial_sum = result.digits[i] + rhs.digits[i] + carry;
            result.digits[i] = partial_sum % 10;
            carry = partial_sum >= 10;
        }
        else if (i < rhs.digits.size()) {
            int partial_sum = rhs.digits[i] + carry;
            result.digits.push_back(partial_sum % 10);
            carry = partial_sum >= 10;
        }
        else {
            result.digits.push_back(carry);
            break;
        }

        ++i;
    }

    return result;
}

BigInt BigInt::operator-(const BigInt &rhs) const {
    assert(false);
}

BigInt BigInt::operator+() {
    return *this;
}

BigInt BigInt::operator-() {
    negative = negative ? false : true;
    return *this;
}

std::string BigInt::to_string() const {
    std::string s_out;

    if (is_negative()) {
        s_out.push_back('-');
    }

    for (auto it = digits.rbegin(); it != digits.rend(); ++it) {
        s_out.push_back(*it + '0');
    }

    return s_out;
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
