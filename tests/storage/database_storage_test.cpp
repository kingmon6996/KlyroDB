#include <gtest/gtest.h>
#include "klyro/api/database.hpp"
#include "klyro/storage/disk_manager.hpp" // included to corrupt file manually
#include <filesystem>
#include <fstream>
#include <vector>

using namespace klyro;

class DatabaseStorageTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_path = "db_test.klyro";
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
};

TEST_F(DatabaseStorageTest, CreateOpenClose) {
    auto db = Database::create(test_path.string()).value();
    EXPECT_TRUE(db.is_open());
    db.close();
    EXPECT_FALSE(db.is_open());
    
    auto db2 = Database::open(test_path.string()).value();
    EXPECT_TRUE(db2.is_open());
}

TEST_F(DatabaseStorageTest, DetectsCorruption) {
    {
        auto db = Database::create(test_path.string()).value();
        db.close();
    }
    
    // Corrupt magic bytes manually
    {
        std::fstream fs(test_path, std::ios::in | std::ios::out | std::ios::binary);
        fs.seekp(0);
        fs.write("BADMAGIC", 8);
    }
    
    auto res = Database::open(test_path.string());
    EXPECT_FALSE(res);
    EXPECT_EQ(res.error(), Status::Corruption);
}
