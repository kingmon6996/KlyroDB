#include <gtest/gtest.h>
#include "klyro/types/decimal.hpp"

using namespace klyro::types;

TEST(TypeDecimalTest, Initialization) {
    Decimal d1(1234, 2); // 12.34
    EXPECT_EQ(d1.coefficient(), 1234);
    EXPECT_EQ(d1.scale(), 2);
    EXPECT_EQ(d1.to_string(), "12.34");

    Decimal d2(-5, 0); // -5
    EXPECT_EQ(d2.to_string(), "-5");

    Decimal d3(42, 4); // 0.0042
    EXPECT_EQ(d3.to_string(), "0.0042");

    Decimal d4(0, 5); // 0.00000
    EXPECT_EQ(d4.to_string(), "0.00000");
}

TEST(TypeDecimalTest, Equality) {
    Decimal d1(120, 1); // 12.0
    Decimal d2(1200, 2); // 12.00
    Decimal d3(12, 0); // 12
    Decimal d4(121, 1); // 12.1

    EXPECT_EQ(d1, d2);
    EXPECT_EQ(d1, d3);
    EXPECT_NE(d1, d4);
}

TEST(TypeDecimalTest, Comparison) {
    Decimal d1(1234, 2); // 12.34
    Decimal d2(124, 1);  // 12.4
    
    EXPECT_LT(d1, d2);
    EXPECT_GT(d2, d1);
}

TEST(TypeDecimalTest, Addition) {
    Decimal d1(1234, 2); // 12.34
    Decimal d2(56, 1);   // 5.6
    
    auto res = d1 + d2; // Should be 17.94
    EXPECT_EQ(res.to_string(), "17.94");
    
    // 0.1 + 0.2 exactness
    Decimal pt1(1, 1); // 0.1
    Decimal pt2(2, 1); // 0.2
    auto pt3 = pt1 + pt2; // 0.3
    Decimal pt3_expected(3, 1);
    EXPECT_EQ(pt3, pt3_expected);
}

TEST(TypeDecimalTest, Multiplication) {
    Decimal d1(12, 1); // 1.2
    Decimal d2(3, 1);  // 0.3
    auto res = d1 * d2; // 0.36
    EXPECT_EQ(res.to_string(), "0.36");
}
