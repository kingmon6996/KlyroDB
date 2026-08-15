#include <gtest/gtest.h>
#include "klyro/api/database.hpp"
#include "klyro/core/status.hpp"

using namespace klyro;

TEST(DatabaseSmokeTest, OpenIsNotImplemented) {
    auto result = Database::open("test.klyro");
    EXPECT_FALSE(result);
    EXPECT_EQ(result.error(), Status::Unsupported);
}

TEST(DatabaseSmokeTest, CreateIsNotImplemented) {
    auto result = Database::create("new_test.klyro");
    EXPECT_FALSE(result);
    EXPECT_EQ(result.error(), Status::Unsupported);
}
