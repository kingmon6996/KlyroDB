#include <gtest/gtest.h>
#include "klyro/storage/disk_manager.hpp"
#include <filesystem>

using namespace klyro;
using namespace klyro::storage;

class DiskManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_path = "dm_test.klyro";
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

TEST_F(DiskManagerTest, CreateAndFormat) {
    auto dm_res = DiskManager::create(test_path, config);
    EXPECT_TRUE(dm_res);
    
    auto dm = std::move(dm_res.value());
    EXPECT_EQ(dm->file_size(), config.page_size()); // Page 0 created
    
    const auto& header = dm->get_database_header();
    EXPECT_EQ(header.format_version, 1);
    EXPECT_EQ(header.page_size, config.page_size());
}

TEST_F(DiskManagerTest, AllocatePage) {
    auto dm = DiskManager::create(test_path, config).value();
    
    auto id1 = dm->allocate_page().value();
    EXPECT_EQ(id1.value(), 1);
    EXPECT_EQ(dm->file_size(), 2 * config.page_size());
    
    auto id2 = dm->allocate_page().value();
    EXPECT_EQ(id2.value(), 2);
}

TEST_F(DiskManagerTest, WriteAndReadPage) {
    auto dm = DiskManager::create(test_path, config).value();
    auto id = dm->allocate_page().value();
    
    Page p1(id, config.page_size());
    p1.payload_span()[0] = std::byte{0xFF};
    
    EXPECT_TRUE(dm->write_page(p1));
    
    auto p2 = dm->read_page(id).value();
    EXPECT_EQ(p2.payload_span()[0], std::byte{0xFF});
}
