#include <cassert>
#include <cmath> // std::floor
#include <exception> // std::invalid_argument
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
// REQUIRES: This method assumes that result is an empty vector to which digits
//           from the result will be appended as they are computed.
static void add(const std::vector<int>& lhs,
                const std::vector<int>& rhs,
                std::vector<int>& result, const int base) {
    assert(result.size() == 0);
    size_t i = 0;
    size_t j = 0;
    int carry = 0;
    while (i < lhs.size() || j < rhs.size()) {
        int partial_sum = carry;
        if (i < lhs.size()) {
            partial_sum += lhs[i];
            ++i;
        }
        if (j < rhs.size()) {
            partial_sum += rhs[j];
            ++j;
        }
        result.push_back(partial_sum % base);
        carry = int(partial_sum / base);
    }
    if (carry) {
        result.push_back(carry);
    }
}

// base routine for subtracting two nonnegative integers
// REQUIRES: This method assumes that result is an empty vector to which digits
//           from the result will be appended as they are computed.
static void subtract(const std::vector<int>& lhs,
                     const std::vector<int>& rhs,
                     std::vector<int>& result, const int base) {
    assert(result.size() == 0);
    size_t i = 0;
    size_t j = 0;
    int borrow = 0;
    while (i < lhs.size() || j < rhs.size()) {
        int partial_sub = borrow;
        if (i < lhs.size()) {
            partial_sub += lhs[i];
            ++i;
        }
        if (j < rhs.size()) {
            partial_sub -= rhs[j];
            ++j;
        }
        result.push_back(mod(partial_sub, base));
        borrow = int(std::floor(float(partial_sub) / base));
    }
    assert(borrow == 0);
    rem_lzeros(result);
}

// base routine for mutliplying two nonnegative integers
// REQUIRES: This method assumes that result is an empty vector to which digits
//           from the result will be appended as they are computed.
static void multiply(const std::vector<int>& lhs,
                     const std::vector<int>& rhs,
                     std::vector<int>& result, const int base) {
    assert(result.size() == 0);

    int k;
    for (size_t i = 0; i < lhs.size() + rhs.size(); ++i) {
        result.push_back(0);
    }
    for (size_t j = 0; j < rhs.size(); ++j) {
        k = 0;
        for (size_t i = 0; i < lhs.size(); ++i) {
            int t = lhs[i] * rhs[j] + result[i + j] + k;
            result[i + j] = mod(t, base);
            k = int(t / base);
        }
        result[lhs.size() + j] = k;
    }
    rem_lzeros(result);
}

// a simpler division algorithm for the case when the divisor is
// of single precision (i.e. 1 < divisor < base)
static void divide_single_precision(const std::vector<int>& lhs,
                                      const std::vector<int>& rhs,
                                      std::vector<int>& result,
                                      const int base) {
    assert(rhs.size() == 1 && rhs[0] != 0);
    // there must be a better way to implement this algorithm that doesn't
    // require resizing the result vector before calculating (O(n)) and then
    // iterating through it to compute and insert digits (O(n))
    result.resize(lhs.size());
    assert(result.size() == lhs.size());
    int v = rhs[0];
    int d_partial = 0;
    for (size_t i = lhs.size(); i-- > 0; ) {
        d_partial = d_partial * base + lhs[i];
        int digit = d_partial / v;
        result[i] = digit;
        d_partial -= digit * v;
    }
    rem_lzeros(result);
    // should this function return the remainder somehow??
}

// base routine for dividing two nonnegative integers
// from Knuth, The Art of Computer Programming (Seminumerical Algorithms):
//
// given nonnegative integers
//      u = (u_{m+n-1}...u_{0})_{b}
//      and
//      v = (v{n-1}...v_{0})_{b},
// where the leading digit of v n.e. 0 and n > 1,
// this recipe forms the radix-b quotient
//      floor(u / v) = (q_{m}...q_{0})_{b}
// and the remainder
//      u mod v = (r_{n-1}...r_{0})_{b}.
//
static void divide(const std::vector<int>& lhs,
                   const std::vector<int>& rhs,
                   std::vector<int>& result, const int base) {
    if (rhs.size() == 1) {
        divide_single_precision(lhs, rhs, result, base);
        return;
    }
    else {
        assert(rhs.back() != 0);
        int n = rhs.size();
        int m = lhs.size() - n;

        // this recipe puts the operands through various deformations. I choose
        // to perform these on copies so that the passed-in args stay constant
        std::vector<int> lhs_norm;
        std::vector<int> rhs_norm;
        int d = (base - 1) / rhs.back(); // implicit truncation
        if (d == 1) {
            lhs_norm.push_back(0);
        }
        else {
            multiply(lhs, {d}, lhs_norm, base);
            multiply(rhs, {d}, rhs_norm, base);
            if (lhs_norm.size() == lhs.size()) {
                lhs_norm.push_back(0);
            }
        }
    }
}

// ^^^^^^^^^^ HELPER FUNCTIONS ^^^^^^^^^^
//
// vvvvvvvvvv CONSTRUCTORS vvvvvvvvvv

BigInt::BigInt()
    : digits({0}), negative(false) { }

BigInt::BigInt(const std::string& val) {
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

BigInt::BigInt(const char* val)
    : BigInt(std::string(val)) { }

BigInt::BigInt(const std::vector<int>& digits_in, const bool negative_in)
    : digits(digits_in), negative(negative_in) { }

// assign to a BigInt from a string representation of an integer
//
// * is this a good idea? I am implementing this with convenience
// in mind, for example, to reassign to a previously-created BigInt a:
// a = "12345"; (instead of/in addition to)
// a = BigInt("12345");
//
// Is this confusing more than it is helpful?
BigInt& BigInt::operator=(const std::string& val) {
    return *this = BigInt(val);
}

BigInt::BigInt(const int val) {
    int n = val >= 0 ? val : -val;
    while (n > 0) {
        int dig = n % 10;
        n = (n - dig) / 10;
        digits.push_back(dig);
    }
    negative = val < 0;
}

BigInt& BigInt::operator=(const char* val) {
    return *this = BigInt(std::string(val));
}

// ^^^^^^^^^^ CONSTRUCTORS ^^^^^^^^^^

bool BigInt::is_negative() const {
    return negative;
}

// design question: how long should an unitilialized BigInt be? Should
// a BigInt even have a length attribute at all?
int BigInt::length() const {
    return int(digits.size());
}

// vvvvvvvvvv ARITHMETIC-ASSIGNMENT OPERATORS vvvvvvvvvv

BigInt& BigInt::operator+=(const BigInt& rhs) {
    // define in terms of the overloaded addition operator
    return *this = *this + rhs;
}

BigInt& BigInt::operator-=(const BigInt& rhs) {
    // define in terms of the overloaded subtraction operator
    return *this = *this - rhs;
}

BigInt& BigInt::operator*=(const BigInt& rhs) {
    // define in terms of the overloaded subtraction operator
    return *this = *this * rhs;
}

BigInt& BigInt::operator/=(const BigInt& rhs) {
    assert(false);
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
    else if (this->is_negative() && rhs.is_negative()) {
        result = -rhs - -(*this);
    }
    else {
        if (this->is_negative()) {
            add(this->digits, rhs.digits, result_digs, BASE);
            result = {result_digs, true};
        }
        else {
            add(this->digits, rhs.digits, result_digs, BASE);
            result = {result_digs, false};
        }
    }
    return result;
}

BigInt BigInt::operator*(const BigInt &rhs) const {
    BigInt result;
    std::vector<int> result_digs;
    multiply(this->digits, rhs.digits, result_digs, BASE);
    bool neg = this->is_negative() == rhs.is_negative() ? false : true;
    result = {result_digs, neg};
    return result;
}

BigInt BigInt::operator/(const BigInt &rhs) const {
    BigInt result;
    std::vector<int> result_digs;
    divide(this->digits, rhs.digits, result_digs, BASE);
    bool neg = this->is_negative() == rhs.is_negative() ? false : true;
    result = {result_digs, neg};
    return result;
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
    return *this += BigInt("1");
}

BigInt& BigInt::operator--() {
    return *this -= BigInt("1");
}

BigInt BigInt::operator++(int) {
    BigInt copy = *this;
    *this += BigInt("1");
    return copy;
}

BigInt BigInt::operator--(int) {
    BigInt copy = *this;
    *this -+ BigInt("1");
    return copy;
}

// ^^^^^^^^^^ UNARY OPERATORS ^^^^^^^^^^
//
// vvvvvvvvv COMPARISON OPERATORS vvvvvvvvv

bool BigInt::operator==(const BigInt &rhs) const {
    if (this->digits.size() != rhs.digits.size() ||
        this->is_negative() != rhs.is_negative()) {
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
    if (this->is_negative() && !rhs.is_negative()) {
        return true;
    }
    else if (!this->is_negative() && rhs.is_negative()) {
        return false;
    }
    else { // *this and rhs have the same sign
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
