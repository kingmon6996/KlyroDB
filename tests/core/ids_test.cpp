#include <gtest/gtest.h>
#include "klyro/core/ids.hpp"

using namespace klyro;

TEST(IDsTest, DefaultIsInvalid) {
    PageID p;
    EXPECT_FALSE(p.is_valid());
    EXPECT_EQ(p.value(), PageID::invalid_value());
}

TEST(IDsTest, ValueAssignment) {
    TableID t(42);
    EXPECT_TRUE(t.is_valid());
    EXPECT_EQ(t.value(), 42);
}

TEST(IDsTest, Comparison) {
    RowID r1(10);
    RowID r2(10);
    RowID r3(20);

    EXPECT_EQ(r1, r2);
    EXPECT_NE(r1, r3);
    EXPECT_LT(r1, r3);
    EXPECT_LE(r1, r2);
}

// Note: Accidental mixing of types (e.g. PageID p = TableID(1)) 
// is prevented at compile time by the strong typing, so we don't 
// write a runtime test for it, but we rely on compiler enforcement.
