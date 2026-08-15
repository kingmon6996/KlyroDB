#include <gtest/gtest.h>
#include "klyro/concurrency/lock_manager.hpp"
#include "klyro/concurrency/lock_handle.hpp"
#include <thread>
#include <vector>

using namespace klyro::concurrency;
using namespace klyro::transaction;

TEST(LockManagerTest, BasicSharedExclusive) {
    LockManager lm(4);
    LockResource r1{LockResourceType::Row, 0, 0, 1, 0, 42, 0};
    
    EXPECT_EQ(lm.lock(1, r1, LockMode::Shared), klyro::core::Status::Success);
    EXPECT_EQ(lm.lock(2, r1, LockMode::Shared), klyro::core::Status::Success);
    
    // T3 requests exclusive, should wait (we will simulate timeout to avoid blocking test indefinitely)
    EXPECT_EQ(lm.lock_with_timeout(3, r1, LockMode::Exclusive, std::chrono::milliseconds(10)), klyro::core::Status::LockTimeout);
    
    lm.unlock(1, r1);
    lm.unlock(2, r1);
    
    EXPECT_EQ(lm.lock(3, r1, LockMode::Exclusive), klyro::core::Status::Success);
    EXPECT_EQ(lm.lock_with_timeout(4, r1, LockMode::Shared, std::chrono::milliseconds(10)), klyro::core::Status::LockTimeout);
}

TEST(LockManagerTest, DeadlockDetection) {
    LockManager lm(4);
    LockResource r1{LockResourceType::Row, 0, 0, 1, 0, 42, 0};
    LockResource r2{LockResourceType::Row, 0, 0, 1, 0, 99, 0};
    
    // T1 holds r1, T2 holds r2
    EXPECT_EQ(lm.lock(1, r1, LockMode::Exclusive), klyro::core::Status::Success);
    EXPECT_EQ(lm.lock(2, r2, LockMode::Exclusive), klyro::core::Status::Success);
    
    std::atomic<bool> t1_done{false};
    std::atomic<bool> t2_done{false};
    
    std::thread t1([&]() {
        // T1 waits on r2
        auto res = lm.lock(1, r2, LockMode::Exclusive);
        EXPECT_TRUE(res == klyro::core::Status::DeadlockDetected || res == klyro::core::Status::Success);
        t1_done = true;
    });
    
    std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Ensure T1 is waiting
    
    std::thread t2([&]() {
        // T2 waits on r1 -> Deadlock!
        auto res = lm.lock(2, r1, LockMode::Exclusive);
        EXPECT_TRUE(res == klyro::core::Status::DeadlockDetected || res == klyro::core::Status::Success);
        t2_done = true;
    });
    
    t1.join();
    t2.join();
    
    // One of them should have completed successfully (the other aborted due to deadlock).
    // Or, since it's a closed system, one got DeadlockDetected, which aborted it, meaning the other then proceeded!
    EXPECT_TRUE(t1_done && t2_done);
}

TEST(LockManagerTest, LockUpgrade) {
    LockManager lm(4);
    LockResource r1{LockResourceType::Row, 0, 0, 1, 0, 42, 0};
    
    EXPECT_EQ(lm.lock(1, r1, LockMode::Shared), klyro::core::Status::Success);
    
    // Upgrade to X
    EXPECT_EQ(lm.lock(1, r1, LockMode::Exclusive), klyro::core::Status::Success);
}
