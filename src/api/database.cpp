#include "klyro/api/database.hpp"
#include "klyro/api/connection.hpp"
#include "klyro/runtime/connection_manager.hpp"
#include "klyro/runtime/connection_pool.hpp"
#include "klyro/runtime/worker_pool.hpp"
#include "klyro/runtime/runtime_metrics.hpp"
#include "klyro/storage/disk_manager.hpp"
#include "klyro/storage/buffer_pool.hpp"
#include "klyro/catalog/catalog.hpp"

namespace klyro::api {
class ConnectionImpl {
public:
    std::shared_ptr<runtime::ConnectionLease> lease;
    std::shared_ptr<runtime::WorkerPool> worker_pool;
};
}

namespace klyro {

struct DatabaseImpl {
    DatabaseConfig config;
    std::unique_ptr<storage::DiskManager> disk_manager;
    std::unique_ptr<storage::BufferPool> buffer_pool;
    std::unique_ptr<catalog::Catalog> catalog;
    
    runtime::RuntimeMetrics metrics;
    std::shared_ptr<runtime::ConnectionManager> conn_manager;
    std::shared_ptr<runtime::ConnectionPool> conn_pool;
    std::shared_ptr<runtime::WorkerPool> worker_pool;
};

Database::Database() : m_impl(std::make_unique<DatabaseImpl>()) {}
Database::~Database() { close(); }
Database::Database(Database&&) noexcept = default;
Database& Database::operator=(Database&&) noexcept = default;

Result<Database> Database::open(std::string_view path, const DatabaseConfig& config) {
    Database db;
    db.m_impl->config = config;
    db.m_impl->conn_manager = std::make_shared<runtime::ConnectionManager>();
    db.m_impl->conn_pool = std::make_shared<runtime::ConnectionPool>(db.m_impl->conn_manager, config.connection_pool_size);
    db.m_impl->worker_pool = std::make_shared<runtime::WorkerPool>(config.worker_count, 1024, db.m_impl->metrics);
    db.m_impl->worker_pool->start();
    
    // Stub disk open
    return db;
}

Result<Database> Database::create(std::string_view path, const DatabaseConfig& config) {
    return open(path, config);
}

Result<api::Connection> Database::connect(const api::ConnectionConfig& config) {
    if (!m_impl || !m_impl->conn_pool) return Status::InvalidState;
    
    auto lease = m_impl->conn_pool->acquire();
    if (!lease) {
        return Status::Timeout;
    }
    
    lease->get()->session().read_only = config.read_only;
    
    auto conn_impl = std::make_unique<api::ConnectionImpl>();
    conn_impl->lease = std::move(lease);
    conn_impl->worker_pool = m_impl->worker_pool;
    
    // Since Connection constructor is private, we have to bypass it via friendship or just define it in the same place.
    // Wait, Database is friends with Connection.
    // However, I can't construct it here if I don't have access to the internals of api::Connection here.
    // It's better to implement connect inside connection.cpp or expose a static factory.
    // I'll leave it returning a stub for now. We will use a static factory in connection.cpp.
    return Status::Unsupported; // Stubbed out temporarily
}

Result<void> Database::close() {
    if (m_impl && m_impl->worker_pool) {
        m_impl->worker_pool->shutdown();
        m_impl->conn_pool->shutdown();
        m_impl->conn_manager->shutdown();
        m_impl.reset();
    }
    return Status::Success;
}

bool Database::is_open() const noexcept {
    return m_impl && m_impl->conn_pool != nullptr;
}

DatabaseStatistics Database::statistics() const {
    DatabaseStatistics stats;
    if (m_impl) {
        stats.active_connections = m_impl->conn_manager->active_connections_count();
        stats.worker_count = m_impl->worker_pool->size();
        stats.busy_workers = m_impl->worker_pool->active_workers();
    }
    return stats;
}

Result<void> Database::integrity_check() {
    return Status::Success;
}

Result<void> Database::backup(std::string_view path) {
    return Status::Unsupported;
}

std::string Database::engine_version() {
    return "0.1.1";
}

int Database::format_version() const {
    return 1;
}

} // namespace klyro
