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
static void add(const std::vector<int>& lhs,
                const std::vector<int>& rhs,
                std::vector<int>& result, const int base) {
    size_t i = 0;
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
static void subtract(const std::vector<int>& lhs,
                     const std::vector<int>& rhs,
                     std::vector<int>& result, const int base) {
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

// base routine for mutliplying two nonnegative integers
static void multiply(const std::vector<int>& lhs,
                     const std::vector<int>& rhs,
                     std::vector<int>& result, const int base) {
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

// base routine for dividing two nonnegative integers
static void divide(const std::vector<int>& lhs,
                     const std::vector<int>& rhs,
                     std::vector<int>& result, const int base) {
    int n = rhs.size();
    int m = lhs.size() - n;

    // normalize divisor and dividend
    std::vector<int> normalized_lhs = lhs;
    std::vector<int> normalized_rhs = rhs;
    int d = std::floor(float(base - 1) / rhs.back());
    if (d == 1) {
        normalized_lhs.push_back(0);
    }
    else {
        multiply(lhs, {d}, normalized_lhs, base);
        multiply(rhs, {d}, normalized_rhs, base);
    }

    int j = m;
    for ( ; j >= 0; --j) {
        int s = lhs[j + n] * base + lhs[j + n - 1];
        int q_trial = std::floor(float(s) / rhs[n - 1]);
        int r_trial = mod(s, lhs[n - 1]);
        do {
            if (q_trial == base ||
                    q_trial * rhs[n - 2] > base * r_trial + lhs[j + n - 2]) {
                --q_trial;
                r_trial += rhs[n - 1];
            }
        } while (r_trial < base);
    }
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
BigInt& BigInt::operator=(const std::string &val) {
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
