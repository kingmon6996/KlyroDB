#include <gtest/gtest.h>
#include "klyro/storage/checksum.hpp"
#include <vector>
#include <string_view>

using namespace klyro::storage;

TEST(ChecksumTest, DeterministicOutput) {
    std::string_view text1 = "Hello KlyroDB";
    std::string_view text2 = "Hello KlyroDB";
    
    auto span1 = std::span<const std::byte>(reinterpret_cast<const std::byte*>(text1.data()), text1.size());
    auto span2 = std::span<const std::byte>(reinterpret_cast<const std::byte*>(text2.data()), text2.size());
    
    EXPECT_EQ(calculate_checksum(span1), calculate_checksum(span2));
}

TEST(ChecksumTest, DetectsCorruption) {
    std::string text = "Hello KlyroDB";
    auto span1 = std::span<const std::byte>(reinterpret_cast<const std::byte*>(text.data()), text.size());
    auto csum1 = calculate_checksum(span1);
    
    text[0] = 'h'; // corrupt
    auto span2 = std::span<const std::byte>(reinterpret_cast<const std::byte*>(text.data()), text.size());
    auto csum2 = calculate_checksum(span2);
    
    EXPECT_NE(csum1, csum2);
}
