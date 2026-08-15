#include <gtest/gtest.h>
#include "klyro/storage/buffer_pool.hpp"
#include <filesystem>
#include <chrono>
#include <iostream>
#include <random>

using namespace klyro;
using namespace klyro::storage;

class BufferPoolBenchmark : public ::testing::Test {
protected:
    void SetUp() override {
        test_path = "bp_bench.klyro";
        if (std::filesystem::exists(test_path)) {
            std::filesystem::remove(test_path);
        }
        config.set_buffer_pool_size(1024 * config.page_size()); // 1024 frames (8MB)
    }
    
    void TearDown() override {
        if (std::filesystem::exists(test_path)) {
            std::filesystem::remove(test_path);
        }
    }

    std::filesystem::path test_path;
    Config config;
};

TEST_F(BufferPoolBenchmark, CacheHitPerformance) {
    auto dm = DiskManager::create(test_path, config).value();
    BufferPool pool(std::move(dm), config);
    
    auto h = pool.allocate_page().value();
    PageID pid = h.get().id();
    
    // Drop handle to unpin
    { PageHandle temp = std::move(h); }
    
    constexpr int ITERS = 1'000'000;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERS; ++i) {
        auto handle = pool.fetch_page(pid).value();
        // Implicit unpin
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "[ BENCHMARK ] 1M Cache Hits took " << duration.count() << " ms\n";
    std::cout << "[ BENCHMARK ] Cache Hit Ratio: " << pool.stats().cache_hit_ratio() << "\n";
}

TEST_F(BufferPoolBenchmark, RandomAccessEvictionPressure) {
    auto dm = DiskManager::create(test_path, config).value();
    BufferPool pool(std::move(dm), config);
    
    constexpr int NUM_PAGES = 5000; // 5x the pool size
    std::vector<PageID> pids;
    pids.reserve(NUM_PAGES);
    
    for (int i = 0; i < NUM_PAGES; ++i) {
        pids.push_back(pool.allocate_page().value().get().id());
    }
    
    constexpr int ITERS = 100'000;
    std::mt19937 gen(42);
    std::uniform_int_distribution<> dis(0, NUM_PAGES - 1);
    
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < ITERS; ++i) {
        auto handle = pool.fetch_page(pids[dis(gen)]).value();
        if (i % 10 == 0) {
            handle.mark_dirty();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "[ BENCHMARK ] 100k Random accesses took " << duration.count() << " ms\n";
    std::cout << "[ BENCHMARK ] Cache Hit Ratio: " << pool.stats().cache_hit_ratio() << "\n";
    std::cout << "[ BENCHMARK ] Evictions: " << pool.stats().evictions.load() << "\n";
    std::cout << "[ BENCHMARK ] Flushes: " << pool.stats().flushes.load() << "\n";
}
