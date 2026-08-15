#include <gtest/gtest.h>
#include "klyro/core/result.hpp"

using namespace klyro;

TEST(ResultTest, SuccessWithValue) {
    Result<int> res(42);
    EXPECT_TRUE(res);
    EXPECT_EQ(res.value(), 42);
}

TEST(ResultTest, FailureWithValue) {
    Result<int> res(Status::NotFound);
    EXPECT_FALSE(res);
    EXPECT_EQ(res.error(), Status::NotFound);
}

TEST(ResultTest, VoidSuccess) {
    auto res = Result<void>::success();
    EXPECT_TRUE(res);
}

TEST(ResultTest, VoidFailure) {
    Result<void> res(Status::IOError);
    EXPECT_FALSE(res);
    EXPECT_EQ(res.error(), Status::IOError);
}

TEST(ResultTest, MoveSemantics) {
    struct MoveOnly {
        MoveOnly() = default;
        MoveOnly(const MoveOnly&) = delete;
        MoveOnly& operator=(const MoveOnly&) = delete;
        MoveOnly(MoveOnly&&) = default;
        MoveOnly& operator=(MoveOnly&&) = default;
        int val = 10;
    };

    Result<MoveOnly> res(MoveOnly{});
    EXPECT_TRUE(res);
    
    MoveOnly moved = std::move(res).value();
    EXPECT_EQ(moved.val, 10);
}
