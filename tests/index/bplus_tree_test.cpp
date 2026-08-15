#include <gtest/gtest.h>
#include "klyro/index/bplus_tree.hpp"
#include "klyro/index/bplus_tree_iterator.hpp"
#include "klyro/storage/disk_manager.hpp"
#include "klyro/storage/buffer_pool.hpp"
#include <filesystem>

using namespace klyro::index;
using namespace klyro::storage;
using namespace klyro::types;

class BPlusTreeTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::string db_path = "test_bplus_tree.klyro";
        if (std::filesystem::exists(db_path)) {
            std::filesystem::remove(db_path);
        }
        
        disk_mgr = std::make_unique<DiskManager>();
        auto create_res = disk_mgr->create_database(db_path);
        ASSERT_TRUE(create_res);
        
        buffer_pool = std::make_unique<BufferPool>(disk_mgr.get(), 50); // 50 pages for test
        
        IndexMetadata meta;
        meta.index_id = 1;
        meta.name = "idx_test";
        meta.is_unique = false;
        meta.key_types = {TypeID::Integer};
        
        auto tree_res = BPlusTree::create(buffer_pool.get(), meta);
        ASSERT_TRUE(tree_res);
        tree = std::move(tree_res.value());
    }

    void TearDown() override {
        tree.reset();
        buffer_pool.reset();
        disk_mgr.reset();
        if (std::filesystem::exists("test_bplus_tree.klyro")) {
            std::filesystem::remove("test_bplus_tree.klyro");
        }
    }

    std::unique_ptr<DiskManager> disk_mgr;
    std::unique_ptr<BufferPool> buffer_pool;
    std::unique_ptr<BPlusTree> tree;
};

TEST_F(BPlusTreeTest, BasicInsertAndFind) {
    IndexKey k1(Value(static_cast<std::int32_t>(10)));
    RecordID r1(PageID(1), 0);
    
    ASSERT_TRUE(tree->insert(k1, r1));
    
    auto res = tree->find(k1);
    ASSERT_TRUE(res);
    EXPECT_EQ(res.value().size(), 1);
    EXPECT_EQ(res.value()[0], r1);
}

TEST_F(BPlusTreeTest, MultipleInsertsAndSplits) {
    // Insert enough to cause leaf and internal splits
    const int NUM_ENTRIES = 5000;
    
    for (int i = 0; i < NUM_ENTRIES; ++i) {
        IndexKey k(Value(static_cast<std::int32_t>(i)));
        RecordID r(PageID(i / 100), i % 100);
        ASSERT_TRUE(tree->insert(k, r));
    }
    
    // Verify tree height has grown
    EXPECT_GT(tree->metadata().tree_height, 1);
    
    // Verify all can be found
    for (int i = 0; i < NUM_ENTRIES; ++i) {
        IndexKey k(Value(static_cast<std::int32_t>(i)));
        auto res = tree->find(k);
        ASSERT_TRUE(res);
        EXPECT_EQ(res.value().size(), 1);
        EXPECT_EQ(res.value()[0].page_id().value(), i / 100);
        EXPECT_EQ(res.value()[0].slot_id(), i % 100);
    }
}

TEST_F(BPlusTreeTest, IteratorRangeScan) {
    for (int i = 1; i <= 100; ++i) {
        IndexKey k(Value(static_cast<std::int32_t>(i * 10))); // 10, 20, 30...
        RecordID r(PageID(1), i);
        ASSERT_TRUE(tree->insert(k, r));
    }
    
    // Scan from 45 -> Should start at 50
    IndexKey start_key(Value(static_cast<std::int32_t>(45)));
    auto it_res = tree->lower_bound(start_key);
    ASSERT_TRUE(it_res);
    
    auto it = std::move(it_res.value());
    ASSERT_TRUE(it.is_valid());
    
    EXPECT_EQ(it.key().at(0).get<std::int32_t>(), 50);
    
    int expected = 50;
    int count = 0;
    while (it.is_valid()) {
        EXPECT_EQ(it.key().at(0).get<std::int32_t>(), expected);
        expected += 10;
        count++;
        it.next();
    }
    EXPECT_EQ(count, 100 - 5 + 1); // 50 through 1000 inclusive
}

TEST_F(BPlusTreeTest, DeleteExact) {
    IndexKey k(Value(static_cast<std::int32_t>(42)));
    RecordID r1(PageID(1), 1);
    RecordID r2(PageID(1), 2);
    
    ASSERT_TRUE(tree->insert(k, r1));
    ASSERT_TRUE(tree->insert(k, r2));
    
    auto res1 = tree->find(k);
    ASSERT_EQ(res1.value().size(), 2);
    
    // Delete one of the duplicates
    auto rem_res = tree->remove(k, r1);
    ASSERT_TRUE(rem_res);
    EXPECT_TRUE(rem_res.value());
    
    auto res2 = tree->find(k);
    ASSERT_EQ(res2.value().size(), 1);
    EXPECT_EQ(res2.value()[0], r2);
}
