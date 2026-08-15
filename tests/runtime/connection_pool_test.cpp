#include "klyro/runtime/connection_pool.hpp"
#include "klyro/runtime/connection_lease.hpp"
#include <gtest/gtest.h>
#include <thread>
#include <vector>

using namespace klyro::runtime;

TEST(ConnectionPoolTest, AcquireRelease) {
    auto manager = std::make_shared<ConnectionManager>();
    auto pool = std::make_shared<ConnectionPool>(manager, 5);
    
    auto lease1 = pool->acquire();
    EXPECT_NE(lease1, nullptr);
    EXPECT_EQ(manager->active_connections_count(), 1);
    
    auto lease2 = pool->acquire();
    EXPECT_NE(lease2, nullptr);
    EXPECT_EQ(manager->active_connections_count(), 2);
    
    // Release both
    lease1.reset();
    lease2.reset();
    
    EXPECT_EQ(manager->active_connections_count(), 2);
    
    // Acquire again, should reuse
    auto lease3 = pool->acquire();
    EXPECT_NE(lease3, nullptr);
    EXPECT_EQ(manager->active_connections_count(), 2);
}

TEST(ConnectionPoolTest, MaxConnectionsBlocking) {
    auto manager = std::make_shared<ConnectionManager>();
    auto pool = std::make_shared<ConnectionPool>(manager, 2);
    
    auto lease1 = pool->acquire();
    auto lease2 = pool->acquire();
    
    auto start_time = std::chrono::steady_clock::now();
    
    std::thread t([&]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lease1.reset(); // Release one connection
    });
    
    // This should block until lease1 is released
    auto lease3 = pool->acquire(std::chrono::milliseconds(1000));
    
    auto end_time = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    EXPECT_NE(lease3, nullptr);
    EXPECT_GE(duration.count(), 80); // Should have waited ~100ms
    
    t.join();
}
