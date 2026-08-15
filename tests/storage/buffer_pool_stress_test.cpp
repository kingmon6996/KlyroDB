#include <gtest/gtest.h>
#include "klyro/storage/buffer_pool.hpp"
#include <filesystem>
#include <thread>
#include <vector>
#include <random>
#include <atomic>

using namespace klyro;
using namespace klyro::storage;

class BufferPoolStressTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_path = "bp_stress.klyro";
        if (std::filesystem::exists(test_path)) {
            std::filesystem::remove(test_path);
        }
        config.set_buffer_pool_size(64 * config.page_size()); // 64 frames
    }
    
    void TearDown() override {
        if (std::filesystem::exists(test_path)) {
            std::filesystem::remove(test_path);
        }
    }

    std::filesystem::path test_path;
    Config config;
};

TEST_F(BufferPoolStressTest, ConcurrentReadsAndWrites) {
    auto dm = DiskManager::create(test_path, config).value();
    BufferPool pool(std::move(dm), config);
    
    constexpr int NUM_PAGES = 1000;
    constexpr int NUM_THREADS = 8;
    constexpr int OPS_PER_THREAD = 10000;
    
    std::vector<PageID> page_ids;
    for (int i = 0; i < NUM_PAGES; ++i) {
        auto h = pool.allocate_page().value();
        page_ids.push_back(h.get().id());
        h.get_mut().payload_span()[0] = std::byte{0};
    }
    
    std::atomic<bool> start{false};
    std::vector<std::thread> threads;
    
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back([&, i]() {
            std::mt19937 gen(i); // deterministic per thread
            std::uniform_int_distribution<> dis(0, NUM_PAGES - 1);
            std::uniform_int_distribution<> op_dis(0, 100);
            
            while (!start.load()) { std::this_thread::yield(); }
            
            for (int j = 0; j < OPS_PER_THREAD; ++j) {
                int page_idx = dis(gen);
                PageID pid = page_ids[page_idx];
                
                auto h_res = pool.fetch_page(pid);
                if (h_res) {
                    auto h = std::move(h_res.value());
                    
                    int op = op_dis(gen);
                    if (op < 20) { // 20% writes
                        h.get_mut().payload_span()[0] = static_cast<std::byte>(i);
                    } else { // 80% reads
                        [[maybe_unused]] auto val = h.get().payload_span()[0];
                    }
                }
            }
        });
    }
    
    start.store(true);
    for (auto& t : threads) {
        t.join();
    }
    
    auto stats = pool.stats();
    EXPECT_GT(stats.cache_hits.load(), 0);
    EXPECT_GT(stats.cache_misses.load(), 0);
}
