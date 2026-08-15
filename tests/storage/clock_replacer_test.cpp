#include <gtest/gtest.h>
#include "klyro/storage/clock_replacer.hpp"

using namespace klyro;
using namespace klyro::storage;

TEST(ClockReplacerTest, BasicEviction) {
    ClockReplacer replacer(3);
    
    // Nothing added/pinned yet, but frames are marked empty initially.
    // We expect nullopt because no unpinned valid frames exist.
    EXPECT_FALSE(replacer.victim().has_value());

    // Pin frames
    replacer.pin(FrameID(0));
    replacer.pin(FrameID(1));
    replacer.pin(FrameID(2));

    // Still nothing evictable
    EXPECT_FALSE(replacer.victim().has_value());

    // Unpin 0 and 1, don't set reference bit
    replacer.unpin(FrameID(0));
    replacer.unpin(FrameID(1));

    // The first victim should be 0 because clock hand is at 0
    auto v1 = replacer.victim();
    ASSERT_TRUE(v1.has_value());
    EXPECT_EQ(v1.value().value(), 0);
    
    // 0 is now considered "empty/transit" until repinned, so next victim should be 1
    auto v2 = replacer.victim();
    ASSERT_TRUE(v2.has_value());
    EXPECT_EQ(v2.value().value(), 1);
}

TEST(ClockReplacerTest, ReferenceBitSecondChance) {
    ClockReplacer replacer(3);

    replacer.pin(FrameID(0));
    replacer.pin(FrameID(1));
    replacer.pin(FrameID(2));

    // Unpin all
    replacer.unpin(FrameID(0));
    replacer.unpin(FrameID(1));
    replacer.unpin(FrameID(2));

    // Access 0 and 1, they get second chances
    replacer.record_access(FrameID(0));
    replacer.record_access(FrameID(1));

    // Hand is at 0. It sees 0 has ref bit -> clears ref bit, moves to 1
    // It sees 1 has ref bit -> clears ref bit, moves to 2
    // It sees 2 has NO ref bit -> evicts 2
    auto v1 = replacer.victim();
    ASSERT_TRUE(v1.has_value());
    EXPECT_EQ(v1.value().value(), 2);

    // Now hand is at 0 again. 0 has no ref bit now, so it should be evicted.
    auto v2 = replacer.victim();
    ASSERT_TRUE(v2.has_value());
    EXPECT_EQ(v2.value().value(), 0);
}
