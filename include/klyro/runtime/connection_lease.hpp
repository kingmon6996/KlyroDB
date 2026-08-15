#ifndef KLYRO_RUNTIME_CONNECTION_LEASE_HPP
#define KLYRO_RUNTIME_CONNECTION_LEASE_HPP

#include "klyro/runtime/connection_pool.hpp"

namespace klyro::runtime {

class ConnectionLease {
public:
    ConnectionLease(std::shared_ptr<ConnectionPool> pool, std::shared_ptr<Connection> conn)
        : m_pool(std::move(pool)), m_conn(std::move(conn)) {}
        
    ~ConnectionLease() {
        if (m_conn && m_pool) {
            m_conn->reset_for_pool();
            m_pool->release(std::move(m_conn));
        }
    }
    
    // Non-copyable
    ConnectionLease(const ConnectionLease&) = delete;
    ConnectionLease& operator=(const ConnectionLease&) = delete;
    
    Connection* get() const noexcept { return m_conn.get(); }
    Connection* operator->() const noexcept { return m_conn.get(); }
    Connection& operator*() const noexcept { return *m_conn; }
    
private:
    std::shared_ptr<ConnectionPool> m_pool;
    std::shared_ptr<Connection> m_conn;
};

} // namespace klyro::runtime

#endif // KLYRO_RUNTIME_CONNECTION_LEASE_HPP
