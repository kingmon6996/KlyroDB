#include <gtest/gtest.h>
#include "klyro/storage/table_heap.hpp"
#include "klyro/storage/buffer_pool.hpp"
#include "klyro/storage/disk_manager.hpp"
#include <filesystem>
#include <string>

using namespace klyro::storage;
using namespace klyro::types;

class TableHeapTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::string db_path = "test_table_heap.klyro";
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
        if (std::filesystem::exists("test_table_heap.klyro")) {
            std::filesystem::remove("test_table_heap.klyro");
        }
    }

    std::unique_ptr<DiskManager> disk_mgr;
    std::unique_ptr<BufferPool> buffer_pool;
};

TEST_F(TableHeapTest, BasicInsertAndRetrieve) {
    auto heap_res = TableHeap::create(buffer_pool.get());
    ASSERT_TRUE(heap_res);
    auto heap = std::move(heap_res.value());
    
    TupleLayout layout;
    layout.add_column(TypeID::Integer);
    layout.add_column(TypeID::Text);
    
    std::vector<Value> fields;
    fields.push_back(Value(static_cast<std::int32_t>(100)));
    fields.push_back(Value("Test String", TypeID::Text));
    Record rec(std::move(fields));
    
    auto id_res = heap.insert(rec, layout);
    ASSERT_TRUE(id_res);
    
    auto get_res = heap.get(id_res.value(), layout);
    ASSERT_TRUE(get_res);
    
    EXPECT_EQ(rec, get_res.value());
}

TEST_F(TableHeapTest, SpanningMultiplePages) {
    auto heap_res = TableHeap::create(buffer_pool.get());
    ASSERT_TRUE(heap_res);
    auto heap = std::move(heap_res.value());
    
    TupleLayout layout;
    layout.add_column(TypeID::Text);
    
    std::vector<RecordID> ids;
    std::string big_string(1000, 'A'); // 1KB string
    
    // Insert 10 records. Assuming 4KB pages, this will definitely span multiple pages
    for (int i = 0; i < 10; ++i) {
        std::vector<Value> fields;
        fields.push_back(Value(big_string, TypeID::Text));
        Record rec(std::move(fields));
        
        auto id_res = heap.insert(rec, layout);
        ASSERT_TRUE(id_res);
        ids.push_back(id_res.value());
    }
    
    // Verify we have multiple pages
    bool different_pages = false;
    for (size_t i = 1; i < ids.size(); ++i) {
        if (ids[i].page_id() != ids[0].page_id()) {
            different_pages = true;
            break;
        }
    }
    EXPECT_TRUE(different_pages);
    
    // Retrieve all
    for (int i = 0; i < 10; ++i) {
        auto get_res = heap.get(ids[i], layout);
        ASSERT_TRUE(get_res);
        EXPECT_EQ(get_res.value().field(0).get<std::string>(), big_string);
    }
}
