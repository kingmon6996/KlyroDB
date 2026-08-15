#include "klyro/api/connection.hpp"
#include "klyro/runtime/connection_lease.hpp"
#include "klyro/runtime/worker_pool.hpp"

namespace klyro::api {

class ConnectionImpl {
public:
    std::shared_ptr<runtime::ConnectionLease> lease;
    std::shared_ptr<runtime::WorkerPool> worker_pool;
};

// Factory wrapper for Database::connect
Result<Connection> create_connection(std::shared_ptr<runtime::ConnectionLease> lease, std::shared_ptr<runtime::WorkerPool> pool) {
    auto impl = std::make_unique<ConnectionImpl>();
    impl->lease = std::move(lease);
    impl->worker_pool = std::move(pool);
    return Connection(std::move(impl));
}

Connection::Connection(std::unique_ptr<ConnectionImpl> impl) : m_impl(std::move(impl)) {}
Connection::~Connection() { close(); }
Connection::Connection(Connection&&) noexcept = default;
Connection& Connection::operator=(Connection&&) noexcept = default;

Result<Result> Connection::execute(std::string_view sql) {
    return Status::Unsupported;
}

Result<Result> Connection::execute(std::string_view sql, std::span<const types::Value> parameters) {
    return Status::Unsupported;
}

Result<PreparedStatement> Connection::prepare(std::string_view sql) {
    return Status::Unsupported;
}

Result<Transaction> Connection::begin() {
    return Status::Unsupported;
}

Result<Transaction> Connection::transaction() {
    return begin();
}

Result<void> Connection::commit() {
    return Status::Unsupported;
}

Result<void> Connection::rollback() {
    return Status::Unsupported;
}

void Connection::set_autocommit(bool enabled) {
    if (m_impl && m_impl->lease) {
        m_impl->lease->get()->session().autocommit = enabled;
    }
}

void Connection::cancel() {
    if (m_impl && m_impl->lease) {
        m_impl->lease->get()->cancellation_token().cancel();
    }
}

void Connection::close() {
    m_impl.reset();
}

} // namespace klyro::api
