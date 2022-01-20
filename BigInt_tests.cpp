#include "BigInt.h"
#include "unit_test_framework.h"

TEST(test_default_ctor) {
    BigInt a;

    ASSERT_EQUAL(a.to_string(), "0");
    ASSERT_FALSE(a.is_negative());
}

TEST(test_ctor) {
    BigInt a {"12345"};
    ASSERT_EQUAL(a.to_string(), "12345");

    a = BigInt("-12345");
    ASSERT_EQUAL(a.to_string(), "-12345");
    ASSERT_TRUE(a.is_negative());

    BigInt b = "-8";
    ASSERT_EQUAL(b.to_string(), "-8");
    ASSERT_TRUE(b.is_negative());
}

TEST(test_assignment_from_string) {
    BigInt a;
    a = "-12345";
    ASSERT_EQUAL(a.to_string(), "-12345");
    ASSERT_TRUE(a.is_negative());
}

TEST(test_addition_no_carry) {
    BigInt a("111");
    BigInt b("222");
    BigInt c = a + b;
    ASSERT_EQUAL(c.to_string(), "333");
}

TEST(test_addition_w_carry) {
    BigInt a("999");
    BigInt b("2");
    BigInt expected("1001");
    ASSERT_EQUAL(a + b, expected);
}

TEST(test_subtraction_no_carry) {
    BigInt a("333");
    BigInt b("222");
    BigInt expected("111");
    ASSERT_EQUAL(a - b, expected);
}

TEST(test_subtraction_w_carry) {
    BigInt a = "1001";
    BigInt b = "2";
    BigInt expected = "999";
    ASSERT_EQUAL(a - b, expected);
}

TEST(test_add_neg_neg) {
    BigInt a = "-4";
    BigInt b = "-7";
    BigInt expected = "-11";
    ASSERT_EQUAL(a + b, expected);
    ASSERT_EQUAL(b + a, expected);
}

TEST(test_add_pos_neg) {
    BigInt a = "4";
    BigInt b = "-5";
    BigInt expected = "-1";
    ASSERT_EQUAL(a + b, expected);
    ASSERT_EQUAL(b + a, expected);
}

TEST(test_sub_neg_neg) {
    BigInt a = "-3";
    BigInt b = "-11";
    BigInt expected = "8";
    ASSERT_EQUAL(a - b, expected);
    expected = "-8";
    ASSERT_EQUAL(b - a, expected);
}

TEST(test_sub_neg_pos) {
    BigInt a = "-2";
    BigInt b = "999";
    BigInt expected = "-1001";
    ASSERT_EQUAL(a - b, expected);
    expected = "1001";
    ASSERT_EQUAL(b - a, expected);
}

TEST(test_zero_nonnegative) {
    BigInt a = "3";
    BigInt b = a - a; // approach 0 from above
    ASSERT_FALSE(b.is_negative());
    b = -a + a; // approach 0 from below
    ASSERT_FALSE(b.is_negative());
}

TEST(test_comparison_same_sign) {
    BigInt a = "321";
    BigInt b = "320";
    ASSERT_TRUE(b < a);
    ASSERT_TRUE(b <= a);
    ASSERT_TRUE(a > b);
    ASSERT_TRUE(a >= b);
    ASSERT_TRUE(a != b);
    ASSERT_FALSE(a == b);

    a = "1";
    b = "0";
    ASSERT_TRUE(b < a);
    ASSERT_TRUE(b <= a);
    ASSERT_TRUE(a > b);
    ASSERT_TRUE(a >= b);
    ASSERT_TRUE(a != b);
    ASSERT_FALSE(a == b);

    a = "0";
    ASSERT_FALSE(b < a);
    ASSERT_TRUE(b <= a);
    ASSERT_FALSE(a > b);
    ASSERT_TRUE(a >= b);
    ASSERT_FALSE(a != b);
    ASSERT_TRUE(a == b);
}

TEST(test_comparison_opposite_sign) {
    BigInt a = "1";
    BigInt b = "-98765432";
    ASSERT_TRUE(b < a);
    ASSERT_TRUE(b <= a);
    ASSERT_TRUE(a > b);
    ASSERT_TRUE(a >= b);
    ASSERT_TRUE(a != b);
    ASSERT_FALSE(a == b);
}

TEST_MAIN()
