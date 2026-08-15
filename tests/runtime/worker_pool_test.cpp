#include "klyro/runtime/worker_pool.hpp"
#include "klyro/runtime/runtime_metrics.hpp"
#include <gtest/gtest.h>
#include <atomic>

using namespace klyro::runtime;

TEST(WorkerPoolTest, SubmitTasks) {
    RuntimeMetrics metrics;
    WorkerPool pool(4, 100, metrics);
    pool.start();
    
    std::atomic<int> counter{0};
    
    for (int i = 0; i < 100; ++i) {
        QueryTask task(QueryID{0}, nullptr, [&counter](QueryTask&) {
            counter.fetch_add(1, std::memory_order_relaxed);
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        });
        pool.submit(std::move(task));
    }
    
    // Wait for tasks to complete
    while (metrics.total_queries.load() < 100) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    EXPECT_EQ(counter.load(), 100);
    EXPECT_EQ(metrics.successful_queries.load(), 100);
    
    pool.shutdown();
}

TEST(WorkerPoolTest, ExceptionsCaught) {
    RuntimeMetrics metrics;
    WorkerPool pool(2, 10, metrics);
    pool.start();
    
    QueryTask task(QueryID{0}, nullptr, [](QueryTask&) {
        throw std::runtime_error("Simulated error");
    });
    
    pool.submit(std::move(task));
    
    while (metrics.total_queries.load() < 1) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
    
    EXPECT_EQ(metrics.failed_queries.load(), 1);
    pool.shutdown();
}
