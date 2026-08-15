#include "klyro/api/database.hpp"
#include "klyro/api/connection.hpp"
#include <gtest/gtest.h>

using namespace klyro;
using namespace klyro::api;

TEST(ApiTest, OpenDatabase) {
    DatabaseConfig config;
    config.worker_count = 2;
    auto db_res = Database::open("test.klyro", config);
    EXPECT_TRUE(db_res.has_value());
    
    auto conn_res = db_res.value().connect();
    // Connect should fail with Unsupported for now because of stubs
    EXPECT_FALSE(conn_res.has_value()); 
}
