#include <gtest/gtest.h>
#include "klyro/catalog/catalog.hpp"
#include "klyro/storage/disk_manager.hpp"
#include "klyro/storage/buffer_pool.hpp"
#include <filesystem>

using namespace klyro::catalog;
using namespace klyro::storage;
using namespace klyro::types;
using namespace klyro::index;
using namespace klyro;

class CatalogTest : public ::testing::Test {
protected:
    void SetUp() override {
        std::string db_path = "test_catalog.klyro";
        if (std::filesystem::exists(db_path)) {
            std::filesystem::remove(db_path);
        }
        
        disk_mgr = std::make_unique<DiskManager>();
        auto create_res = disk_mgr->create_database(db_path);
        ASSERT_TRUE(create_res);
        
        buffer_pool = std::make_unique<BufferPool>(disk_mgr.get(), 20);
        catalog = std::make_unique<Catalog>(buffer_pool.get());
        
        PageID root_id;
        auto init_res = catalog->init_new(&root_id);
        ASSERT_TRUE(init_res);
    }

    void TearDown() override {
        catalog.reset();
        buffer_pool.reset();
        disk_mgr.reset();
        if (std::filesystem::exists("test_catalog.klyro")) {
            std::filesystem::remove("test_catalog.klyro");
        }
    }

    std::unique_ptr<DiskManager> disk_mgr;
    std::unique_ptr<BufferPool> buffer_pool;
    std::unique_ptr<Catalog> catalog;
};

TEST_F(CatalogTest, CreateTableAndIndex) {
    // 1. Get Main Schema
    auto schema_res = catalog->find_schema("main");
    ASSERT_TRUE(schema_res);
    SchemaID main_id = schema_res.value().schema_id;
    
    // 2. Define Table Schema
    TableSchema ts;
    ts.add_column(Column(ColumnID(0), "id", TypeID::Integer, 0, false));
    ts.add_column(Column(ColumnID(0), "name", TypeID::VarChar, 1, false));
    
    // 3. Create Table
    auto table_res = catalog->create_table(main_id, "users", ts);
    ASSERT_TRUE(table_res);
    TableID users_table_id = table_res.value();
    
    // 4. Create Index
    auto idx_res = catalog->create_index(users_table_id, "idx_users_id", {ColumnID(1)}, true);
    ASSERT_TRUE(idx_res);
    IndexID idx_users_id = idx_res.value();
    
    // 5. Verify retrieval
    auto get_table = catalog->find_table(main_id, "users");
    ASSERT_TRUE(get_table);
    EXPECT_EQ(get_table.value().table_id, users_table_id);
    EXPECT_EQ(get_table.value().schema.column_count(), 2);
    
    auto get_idx = catalog->find_index(idx_users_id);
    ASSERT_TRUE(get_idx);
    EXPECT_EQ(get_idx.value().name, "idx_users_id");
}

TEST_F(CatalogTest, ValidateConstraints) {
    auto val_res = catalog->validate();
    ASSERT_TRUE(val_res); // Empty catalog is valid
}
