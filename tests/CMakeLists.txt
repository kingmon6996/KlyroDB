include(FetchContent)

FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        release-1.12.1
)
# Prevent googletest from overriding our compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Common function to add a test executable
function(klyro_add_test target_name source_file)
    add_executable(${target_name} ${source_file})
    target_link_libraries(${target_name} PRIVATE klyro GTest::gtest_main)
    klyro_enable_strict_warnings(${target_name})
    klyro_enable_sanitizers(${target_name})
    add_test(NAME ${target_name} COMMAND ${target_name})
endfunction()

# Core tests
klyro_add_test(status_test core/status_test.cpp)
klyro_add_test(result_test core/result_test.cpp)
klyro_add_test(ids_test core/ids_test.cpp)

# Storage tests
klyro_add_test(serialization_test storage/serialization_test.cpp)
klyro_add_test(checksum_test storage/checksum_test.cpp)
klyro_add_test(file_manager_test storage/file_manager_test.cpp)
klyro_add_test(disk_manager_test storage/disk_manager_test.cpp)
klyro_add_test(database_storage_test storage/database_storage_test.cpp)
klyro_add_test(stress_test storage/stress_test.cpp)

# Buffer Pool tests
klyro_add_test(clock_replacer_test storage/clock_replacer_test.cpp)
klyro_add_test(buffer_pool_test storage/buffer_pool_test.cpp)
klyro_add_test(buffer_pool_stress_test storage/buffer_pool_stress_test.cpp)
klyro_add_test(buffer_pool_benchmark storage/buffer_pool_benchmark.cpp)

# Type tests
klyro_add_test(type_value_test types/value_test.cpp)

klyro_add_executable(record_serialization_test storage/record_serialization_test.cpp)
target_link_libraries(record_serialization_test PRIVATE klyro_core GTest::gtest_main)
add_test(NAME RecordSerializationTest COMMAND record_serialization_test)

add_executable(table_page_test storage/table_page_test.cpp)
target_link_libraries(table_page_test PRIVATE klyro_core GTest::gtest_main)
add_test(NAME TablePageTest COMMAND table_page_test)

add_executable(table_heap_test storage/table_heap_test.cpp)
target_link_libraries(table_heap_test PRIVATE klyro_core GTest::gtest_main)
add_test(NAME TableHeapTest COMMAND table_heap_test)

add_executable(bplus_tree_test index/bplus_tree_test.cpp)
target_link_libraries(bplus_tree_test PRIVATE klyro_core GTest::gtest_main)
add_test(NAME BPlusTreeTest COMMAND bplus_tree_test)

add_executable(catalog_test catalog/catalog_test.cpp)
target_link_libraries(catalog_test PRIVATE klyro_core GTest::gtest_main)
add_test(NAME CatalogTest COMMAND catalog_test)

add_executable(lexer_test sql/lexer_test.cpp)
target_link_libraries(lexer_test PRIVATE klyro_core GTest::gtest_main)
add_test(NAME LexerTest COMMAND lexer_test)

add_executable(parser_test sql/parser_test.cpp)
target_link_libraries(parser_test PRIVATE klyro_core GTest::gtest_main)
add_test(NAME ParserTest COMMAND parser_test)

add_executable(mvcc_test transaction/mvcc_test.cpp)
target_link_libraries(mvcc_test PRIVATE klyro_core GTest::gtest_main)
add_test(NAME MvccTest COMMAND mvcc_test)

add_executable(lock_manager_test concurrency/lock_manager_test.cpp)
target_link_libraries(lock_manager_test PRIVATE klyro GTest::gtest_main)
add_test(NAME LockManagerTest COMMAND lock_manager_test)

# Execution tests
klyro_add_test(binder_test execution/binder_test.cpp)
klyro_add_test(planner_test execution/planner_test.cpp)

# Runtime tests
klyro_add_test(connection_pool_test runtime/connection_pool_test.cpp)
klyro_add_test(worker_pool_test runtime/worker_pool_test.cpp)

# API tests
klyro_add_test(api_test api/api_test.cpp)
