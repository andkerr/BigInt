#include "BigInt.h"
#include "unit_test_framework.h"

TEST(test_default_ctor) {
    BigInt a;

    ASSERT_FALSE(a.is_negative());
}

TEST_MAIN()
