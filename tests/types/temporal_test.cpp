#include <gtest/gtest.h>
#include "klyro/types/temporal.hpp"

using namespace klyro::types;

TEST(TypeTemporalTest, DateString) {
    // 0 days since 2000-01-01
    Date d1(0);
    EXPECT_EQ(d1.to_string(), "2000-01-01");
    
    // Simplistic formatting for tests (a real cal lib handles days/months accurately)
    // Here we're just checking that our V1 wrapper retains state correctly.
    Date d2(365);
    EXPECT_EQ(d2.days(), 365);
}

TEST(TypeTemporalTest, TimeString) {
    Time t1(0);
    EXPECT_EQ(t1.to_string(), "00:00:00");
    
    // 1 hour, 1 minute, 1 second, 1 us
    Time t2(3600'000'000LL + 60'000'000LL + 1'000'000LL + 1);
    EXPECT_EQ(t2.to_string(), "01:01:01.000001");
}

TEST(TypeTemporalTest, TimestampAndInterval) {
    Timestamp ts(1234567890LL);
    EXPECT_EQ(ts.microseconds(), 1234567890LL);
    
    Interval i(1, 2, 3);
    EXPECT_EQ(i.months(), 1);
    EXPECT_EQ(i.days(), 2);
    EXPECT_EQ(i.microseconds(), 3);
    EXPECT_EQ(i.to_string(), "1 months 2 days 00:00:00.000003");
}
