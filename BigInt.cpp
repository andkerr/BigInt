#include <algorithm>
#include <cassert>
#include <exception>
#include "BigInt.h"

// base routine for adding two nonnegative integers
static void add(const std::vector<int> &lhs,
                const std::vector<int> &rhs,
                std::vector<int> &result, const int base) {
    int i = 0;
    int carry = 0;
    while (i < lhs.size() || i < rhs.size()) {
        if (i < lhs.size() && i < rhs.size()) {
            int partial_sum = lhs[i] + rhs[i] + carry;
            result[i] = partial_sum % base;
            carry = int(partial_sum / base);
        }
        else if (i < rhs.size()) {
            int partial_sum = rhs[i] + carry;
            result.push_back(partial_sum % base);
            carry = int(partial_sum / base);
        }
        else { // i < lhs.size()
            int partial_sum = lhs[i] + carry;
            result[i] = partial_sum % base;
            carry = int(partial_sum / base);
        }
        ++i;
    }
    if (carry) {
        result.push_back(carry);
    }
}

// base routine for subtracting two nonnegative integers
static void subtract(const std::vector<int> &lhs,
                     const std::vector<int> &rhs,
                     std::vector<int> &result, const int base) {
    assert(false);
}

// base routine for mutliplying two nonegative integers
static void multiply(const std::vector<int> &lhs,
                     const std::vector<int> &rhs,
                     std::vector<int> &result, const int base) {
    assert(false);
}

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

        if (*it < '0' || *it - '0' >= BASE) {
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

BigInt& BigInt::operator+=(const BigInt &rhs) {
    // define in terms of the overloaded addition operator
    return *this = *this + rhs;
}

BigInt BigInt::operator+(const BigInt &rhs) const {
    BigInt result = *this;

    if (!(this->is_negative() || rhs.is_negative())) {
        add(this->digits, rhs.digits, result.digits, BASE);
    }
    else {
        assert(false); // addition with negative BigInts not yet implemented
    }

    return result;
}

BigInt BigInt::operator-(const BigInt &rhs) const {
    BigInt result = *this;

    if(!(this->is_negative() || rhs.is_negative())) {
        if (*this >= rhs) {
            subtract(this->digits, rhs.digits, result.digits, BASE);
        }
        else {
            subtract(rhs.digits, this->digits, result.digits, BASE);
            result.negative = true;
        }
    }
    return result;
}

BigInt BigInt::operator*(const BigInt &rhs) const {
    BigInt result = *this;
    multiply(this->digits, rhs.digits, result.digits, BASE);
    result.negative = this->is_negative() == rhs.is_negative() ? false : true;
    return result;
}

BigInt BigInt::operator/(const BigInt &rhs) const {
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
