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

TEST_MAIN()
