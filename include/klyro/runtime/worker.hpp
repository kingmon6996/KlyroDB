#ifndef KLYRO_RUNTIME_WORKER_HPP
#define KLYRO_RUNTIME_WORKER_HPP

#include "klyro/runtime/task.hpp"
#include "klyro/runtime/runtime_metrics.hpp"
#include "klyro/runtime/memory_resource.hpp"
#include <thread>
#include <atomic>
#include <memory>
#include <exception>

namespace klyro::runtime {

class WorkerPool;

class Worker {
public:
    Worker(std::uint32_t id, WorkerPool* pool, RuntimeMetrics& metrics);
    ~Worker();
    
    // Non-copyable, non-movable
    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;
    
    std::uint32_t id() const noexcept { return m_id; }
    
    // Start the worker thread
    void start();
    
    // Request the worker to stop
    void stop();

private:
    void loop();
    
    std::uint32_t m_id;
    WorkerPool* m_pool;
    RuntimeMetrics& m_metrics;
    
    std::atomic<bool> m_running{false};
    std::unique_ptr<std::thread> m_thread;
    
    // Each worker has a local QueryMemoryResource to reuse memory blocks.
    // In a real system, this could be an arena allocator.
    std::unique_ptr<QueryMemoryResource> m_query_memory;
};

} // namespace klyro::runtime

#endif // KLYRO_RUNTIME_WORKER_HPP
