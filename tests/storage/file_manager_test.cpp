#include <gtest/gtest.h>
#include "klyro/storage/file_manager.hpp"
#include <filesystem>
#include <vector>

using namespace klyro::storage;

class FileManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        test_path = "fm_test.klyro";
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

TEST_F(FileManagerTest, CreateAndOpen) {
    FileManager fm;
    auto res = fm.create_file(test_path);
    EXPECT_TRUE(res);
    EXPECT_TRUE(fm.is_open());
    EXPECT_EQ(fm.file_size().value(), 0);
    fm.close();
    
    auto res2 = fm.open_file(test_path);
    EXPECT_TRUE(res2);
    EXPECT_TRUE(fm.is_open());
}

TEST_F(FileManagerTest, ReadWriteExact) {
    FileManager fm;
    fm.create_file(test_path);
    
    std::vector<std::byte> write_data(100, std::byte{0xAA});
    auto res = fm.write(0, write_data);
    EXPECT_TRUE(res);
    EXPECT_EQ(fm.file_size().value(), 100);
    
    std::vector<std::byte> read_data(100);
    auto res2 = fm.read(0, read_data);
    EXPECT_TRUE(res2);
    EXPECT_EQ(read_data, write_data);
}
