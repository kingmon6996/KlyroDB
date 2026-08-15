#include <gtest/gtest.h>
#include "klyro/storage/serialization.hpp"
#include <vector>

using namespace klyro::storage;

TEST(SerializationTest, WriteReadU16) {
    std::vector<std::byte> buf(2);
    write_u16_le(buf, 0, 0xABCD);
    EXPECT_EQ(read_u16_le(buf, 0), 0xABCD);
}

TEST(SerializationTest, WriteReadU32) {
    std::vector<std::byte> buf(4);
    write_u32_le(buf, 0, 0x12345678);
    EXPECT_EQ(read_u32_le(buf, 0), 0x12345678);
}

TEST(SerializationTest, WriteReadU64) {
    std::vector<std::byte> buf(8);
    write_u64_le(buf, 0, 0x1122334455667788ULL);
    EXPECT_EQ(read_u64_le(buf, 0), 0x1122334455667788ULL);
}
