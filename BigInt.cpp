#include <algorithm>
#include <cassert>
#include <cmath>
#include <exception>
#include "BigInt.h"

// vvvvvvvvvv HELPER FUNCTIONS vvvvvvvvvv

// remove leading zeros
static void rem_lzeros(std::vector<int>& a) {
    while (a.size() > 1 && a.back() == 0) {
        a.pop_back();
    }
}

// compute a mod b
static int mod(const int a, const int b) {
    return a - b * int(std::floor(float(a) / b));
}

// base routine for adding two nonnegative integers
static void add(const std::vector<int>& lhs,
                const std::vector<int>& rhs,
                std::vector<int>& result, const int base) {
    int i = 0;
    int carry = 0;
    while (i < lhs.size() || i < rhs.size()) {
        if (i < lhs.size() && i < rhs.size()) {
            int partial_sum = lhs[i] + rhs[i] + carry;
            result.push_back(partial_sum % base);
            carry = int(partial_sum / base);
        }
        else if (i < rhs.size()) {
            int partial_sum = rhs[i] + carry;
            result.push_back(partial_sum % base);
            carry = int(partial_sum / base);
        }
        else { // i < lhs.size()
            int partial_sum = lhs[i] + carry;
            result.push_back(partial_sum % base);
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
    size_t i = 0;
    int borrow = 0;
    while (i < lhs.size() || i < rhs.size()) {
        if (i < lhs.size() && i < rhs.size()) {
            int partial_sub = lhs[i] - rhs[i] + borrow;
            result.push_back(mod(partial_sub, base));
            borrow = int(std::floor(float(partial_sub) / base));
        }
        else if (i < rhs.size()) {
            int partial_sub = -rhs[i] + borrow;
            result.push_back(mod(partial_sub, base));
            borrow = int(std::floor(float(partial_sub) / base));
        }
        else { // i < lhs.size()
            int partial_sub = lhs[i] + borrow;
            result.push_back(mod(partial_sub, base));
            borrow = int(std::floor(float(partial_sub) / base));
        }
        ++i;
    }
    assert(borrow == 0);
    rem_lzeros(result);
}

// base routine for mutliplying two nonegative integers
static void multiply(const std::vector<int> &lhs,
                     const std::vector<int> &rhs,
                     std::vector<int> &result, const int base) {
    assert(false);
}

// ^^^^^^^^^^ HELPER FUNCTIONS ^^^^^^^^^^
//
// vvvvvvvvvv CONSTRUCTORS vvvvvvvvvv

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
    else {
        negative = false;
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

BigInt::BigInt(const std::vector<int>& digits_in, const bool negative_in)
    : digits(digits_in), negative(negative_in) { };

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

// ^^^^^^^^^^ CONSTRUCTORS ^^^^^^^^^^

bool BigInt::is_negative() const {
    return negative;
}

// vvvvvvvvvv ARITHMETIC-ASSIGNMENT OPERATORS vvvvvvvvvv

BigInt& BigInt::operator+=(const BigInt &rhs) {
    // define in terms of the overloaded addition operator
    return *this = *this + rhs;
}

BigInt& BigInt::operator-=(const BigInt &rhs) {
    // define in terms of the overloaded subtraction operator
    return *this = *this - rhs;
}

// ^^^^^^^^^^ ARITHMETIC-ASSIGNMENT OPERATORS ^^^^^^^^^^
//
// vvvvvvvvvv ARITHMETIC OPERAORS vvvvvvvvvv

BigInt BigInt::operator+(const BigInt &rhs) const {
    BigInt result;
    std::vector<int> result_digs;

    if (!(this->is_negative() || rhs.is_negative())) {
        add(this->digits, rhs.digits, result_digs, BASE);
        result = {result_digs, false};

    }
    else if (this->is_negative() && rhs.is_negative()) {
        add(this->digits, rhs.digits, result_digs, BASE);
        result = {result_digs, true};
    }
    else {
        if (this->is_negative()) {
            result = rhs - (-(*this));
        }
        else {
            result = *this - (-(rhs));
        }
    }

    return result;
}

BigInt BigInt::operator-(const BigInt &rhs) const {
    BigInt result;
    std::vector<int> result_digs;
    if(!(this->is_negative() || rhs.is_negative())) {
        if (*this >= rhs) {
            subtract(this->digits, rhs.digits, result_digs, BASE);
            result = {result_digs, false};
        }
        else {
            subtract(rhs.digits, this->digits, result_digs, BASE);
            result = {result_digs, true};
        }
    }
    else {
        assert(false); // subtraction with negative BigInts not implemented
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

// ^^^^^^^^^^ ARITHMETIC OPERATORS ^^^^^^^^^^
//
// vvvvvvvvvv UNARY OPERATORS vvvvvvvvvv

BigInt BigInt::operator+() const {
    return BigInt(this->digits, this->negative);
}

BigInt BigInt::operator-() const {
    bool negative_in = negative ? false : true;
    return BigInt(this->digits, negative_in);
}

BigInt& BigInt::operator++() {
    assert(false);
}

BigInt& BigInt::operator--() {
    assert(false);
}

BigInt BigInt::operator++(int) {
    assert(false);
}

BigInt BigInt::operator--(int) {
    assert(false);
}

// ^^^^^^^^^^ UNARY OPERATORS ^^^^^^^^^^
//
// vvvvvvvvv COMPARISON OPERATORS vvvvvvvvv

bool BigInt::operator==(const BigInt &rhs) const {
    if (this->digits.size() != rhs.digits.size()) {
        return false;
    }
    else {
        size_t i = 0;
        while (i < this->digits.size() && i < rhs.digits.size()) {
            if (this->digits[i] != rhs.digits[i]) {
                return false;
            }
            ++i;
        }
        return true;
    }
}

bool BigInt::operator!=(const BigInt &rhs) const {
    return !(*this == rhs);
}

bool BigInt::operator<(const BigInt &rhs) const {
    if (this->digits.size() < rhs.digits.size()) {
        return true;
    }
    else if (this->digits.size() > rhs.digits.size()) {
        return false;
    }
    else {
        size_t i = 0;
        while (i < this->digits.size() && i < rhs.digits.size()) {
            if (this->digits[i] < rhs.digits[i]) {
                return true;
            }
            ++i;
        }
        return false;
    }
}

bool BigInt::operator>(const BigInt &rhs) const {
    return !(*this == rhs || *this < rhs);
}

bool BigInt::operator<=(const BigInt &rhs) const {
    return (*this == rhs || *this < rhs);
}

bool BigInt::operator>=(const BigInt &rhs) const {
    return !(*this < rhs);
}

// ^^^^^^^^^^ COMPARISON OPERATORS ^^^^^^^^^^

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
