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
    BigInt a("1001");
    BigInt b("2");
    BigInt expected = "999";
    ASSERT_EQUAL(a - b, expected);
}

TEST_MAIN()
