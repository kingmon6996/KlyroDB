#include <gtest/gtest.h>
#include "klyro/storage/disk_manager.hpp"
#include <filesystem>
#include <vector>

using namespace klyro;
using namespace klyro::storage;

class StressTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_path = "stress_test.klyro";
        if (std::filesystem::exists(test_path)) {
            std::filesystem::remove(test_path);
        }
    }
    
    void TearDown() override {
        if (std::filesystem::exists(test_path)) {
            std::filesystem::remove(test_path);
        }
    }

    std::filesystem::path test_path;
    Config config;
};

// Use a smaller number for unit tests so it doesn't take too long,
// but large enough to prove it scales.
constexpr int NUM_PAGES = 1000;

TEST_F(StressTest, AllocateAndReadManyPages) {
    std::vector<PageID> allocated_ids;
    
    {
        auto dm = DiskManager::create(test_path, config).value();
        for (int i = 0; i < NUM_PAGES; ++i) {
            auto id = dm->allocate_page().value();
            allocated_ids.push_back(id);
            
            Page p(id, config.page_size());
            // Write some deterministic data
            p.payload_span()[0] = static_cast<std::byte>(i % 256);
            p.payload_span()[1] = static_cast<std::byte>((i >> 8) % 256);
            dm->write_page(p);
        }
        dm->close();
    }
    
    {
        auto dm = DiskManager::open(test_path, config).value();
        for (int i = 0; i < NUM_PAGES; ++i) {
            auto p = dm->read_page(allocated_ids[i]).value();
            EXPECT_EQ(p.payload_span()[0], static_cast<std::byte>(i % 256));
            EXPECT_EQ(p.payload_span()[1], static_cast<std::byte>((i >> 8) % 256));
        }
    }
}
