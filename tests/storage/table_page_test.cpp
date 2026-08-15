#include <gtest/gtest.h>
#include "klyro/storage/table_page.hpp"
#include "klyro/storage/buffer_pool.hpp"
#include "klyro/storage/disk_manager.hpp"
#include <filesystem>
#include <string>

using namespace klyro::storage;

class TablePageTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::string db_path = "test_table_page.klyro";
        if (std::filesystem::exists(db_path)) {
            std::filesystem::remove(db_path);
        }
        
        disk_mgr = std::make_unique<DiskManager>();
        auto create_res = disk_mgr->create_database(db_path);
        ASSERT_TRUE(create_res) << create_res.error().message();
        
        buffer_pool = std::make_unique<BufferPool>(disk_mgr.get(), 10);
    }

    void TearDown() override {
        buffer_pool.reset();
        disk_mgr.reset();
        if (std::filesystem::exists("test_table_page.klyro")) {
            std::filesystem::remove("test_table_page.klyro");
        }
    }

    std::unique_ptr<DiskManager> disk_mgr;
    std::unique_ptr<BufferPool> buffer_pool;
};

TEST_F(TablePageTest, BasicInsertAndRetrieve) {
    auto handle_res = buffer_pool->allocate_page();
    ASSERT_TRUE(handle_res);
    
    TablePage page(std::move(handle_res.value()));
    page.init();
    
    std::vector<std::byte> rec1 = {std::byte{1}, std::byte{2}, std::byte{3}};
    std::vector<std::byte> rec2 = {std::byte{4}, std::byte{5}};
    
    auto id1_res = page.insert(rec1);
    ASSERT_TRUE(id1_res);
    auto id2_res = page.insert(rec2);
    ASSERT_TRUE(id2_res);
    
    EXPECT_EQ(id1_res->page_id(), page.page_id());
    EXPECT_EQ(id1_res->slot_id(), 0);
    EXPECT_EQ(id2_res->slot_id(), 1);
    
    auto get1 = page.get(id1_res.value());
    ASSERT_TRUE(get1);
    EXPECT_EQ(get1->size(), 3);
    EXPECT_EQ((*get1)[0], std::byte{1});
    
    auto get2 = page.get(id2_res.value());
    ASSERT_TRUE(get2);
    EXPECT_EQ(get2->size(), 2);
    EXPECT_EQ((*get2)[0], std::byte{4});
}

TEST_F(TablePageTest, DeleteAndCompact) {
    auto handle_res = buffer_pool->allocate_page();
    ASSERT_TRUE(handle_res);
    
    TablePage page(std::move(handle_res.value()));
    page.init();
    
    std::vector<std::byte> r(100, std::byte{0xFF});
    auto id1 = page.insert(r).value();
    auto id2 = page.insert(r).value();
    auto id3 = page.insert(r).value();
    
    std::size_t initial_free = page.free_space();
    
    // Delete middle record
    auto del_res = page.erase(id2);
    ASSERT_TRUE(del_res);
    
    auto get_del = page.get(id2);
    EXPECT_FALSE(get_del); // Should not be found
    EXPECT_EQ(get_del.error(), klyro::Status::NotFound);
    
    // Compaction should reclaim the space
    auto comp_res = page.compact();
    ASSERT_TRUE(comp_res);
    
    std::size_t after_compact_free = page.free_space();
    EXPECT_GT(after_compact_free, initial_free);
    
    // Records 1 and 3 should still be valid at same IDs
    auto get1 = page.get(id1);
    ASSERT_TRUE(get1);
    auto get3 = page.get(id3);
    ASSERT_TRUE(get3);
}

TEST_F(TablePageTest, OversizedRecord) {
    auto handle_res = buffer_pool->allocate_page();
    ASSERT_TRUE(handle_res);
    
    TablePage page(std::move(handle_res.value()));
    page.init();
    
    std::vector<std::byte> r(100000, std::byte{0xFF}); // Way too big
    auto id1 = page.insert(r);
    
    EXPECT_FALSE(id1);
    EXPECT_EQ(id1.error(), klyro::Status::RecordTooLarge);
}
