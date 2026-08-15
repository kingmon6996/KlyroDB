#include <gtest/gtest.h>
#include "klyro/storage/buffer_pool.hpp"
#include "klyro/storage/page_handle.hpp"
#include <filesystem>

using namespace klyro;
using namespace klyro::storage;

class BufferPoolTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_path = "bp_test.klyro";
        if (std::filesystem::exists(test_path)) {
            std::filesystem::remove(test_path);
        }
        
        config.set_buffer_pool_size(3 * config.page_size()); // 3 frames
    }
    
    void TearDown() override {
        if (std::filesystem::exists(test_path)) {
            std::filesystem::remove(test_path);
        }
    }

    std::filesystem::path test_path;
    Config config;
};

TEST_F(BufferPoolTest, BasicFetchAndCacheHit) {
    auto dm = DiskManager::create(test_path, config).value();
    BufferPool pool(std::move(dm), config);
    
    auto handle1 = pool.allocate_page().value();
    PageID id1 = handle1.get().id();
    
    EXPECT_EQ(pool.stats().cache_misses.load(), 1); // Allocation does a fetch (miss)
    EXPECT_EQ(pool.stats().cache_hits.load(), 0);
    
    // Explicitly unpin by dropping handle
    {
        auto temp_handle = std::move(handle1);
    }
    
    // Fetch again, should be a hit
    auto handle2 = pool.fetch_page(id1).value();
    EXPECT_EQ(pool.stats().cache_hits.load(), 1);
    EXPECT_EQ(pool.stats().cache_misses.load(), 1);
}

TEST_F(BufferPoolTest, PinnedPageProtection) {
    auto dm = DiskManager::create(test_path, config).value();
    BufferPool pool(std::move(dm), config);
    
    // Pool size is 3 frames.
    auto h1 = pool.allocate_page().value();
    auto h2 = pool.allocate_page().value();
    auto h3 = pool.allocate_page().value();
    
    // All 3 frames are pinned.
    // Fetching a 4th should fail with exhaustion.
    auto dm_raw = pool.disk_manager();
    PageID id4 = dm_raw->allocate_page().value(); // Allocate bypasses buffer pool for the id
    
    auto h4_res = pool.fetch_page(id4);
    EXPECT_FALSE(h4_res);
    EXPECT_EQ(h4_res.error(), Status::BufferPoolExhausted);
}

TEST_F(BufferPoolTest, DirtyEviction) {
    PageID id1;
    {
        auto dm = DiskManager::create(test_path, config).value();
        BufferPool pool(std::move(dm), config);
        
        auto h1 = pool.allocate_page().value();
        id1 = h1.get().id();
        h1.get_mut().payload_span()[0] = std::byte{0x42};
        
        auto h2 = pool.allocate_page().value();
        auto h3 = pool.allocate_page().value();
        
        // Unpin h1
        PageHandle temp = std::move(h1);
        // temp goes out of scope, h1 unpinned.
    } // BufferPool destroyed, flushes all
    
    // Now reopen
    auto dm = DiskManager::open(test_path, config).value();
    BufferPool pool(std::move(dm), config);
    
    auto h1 = pool.fetch_page(id1).value();
    EXPECT_EQ(h1.get().payload_span()[0], std::byte{0x42});
}
