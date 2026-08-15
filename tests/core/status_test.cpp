#include <gtest/gtest.h>
#include "klyro/core/status.hpp"
#include <string_view>

using namespace klyro;

TEST(StatusTest, ToStringConversion) {
    EXPECT_EQ(to_string(Status::OK), "OK");
    EXPECT_EQ(to_string(Status::NotFound), "NotFound");
    EXPECT_EQ(to_string(Status::AlreadyExists), "AlreadyExists");
    EXPECT_EQ(to_string(Status::IOError), "IOError");
    EXPECT_EQ(to_string(static_cast<Status>(999)), "Unknown");
}
