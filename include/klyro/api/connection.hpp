#ifndef KLYRO_API_CONNECTION_HPP
#define KLYRO_API_CONNECTION_HPP

#include "klyro/core/result.hpp"
#include "klyro/api/result.hpp"
#include "klyro/api/transaction.hpp"
#include "klyro/api/prepared_statement.hpp"
#include <span>
#include <memory>
#include <string_view>

namespace klyro::api {

struct ConnectionConfig {
    bool read_only{false};
};

class ConnectionImpl;

class Connection {
public:
    Connection(Connection&&) noexcept;
    Connection& operator=(Connection&&) noexcept;
    ~Connection();

    // Non-copyable
    Connection(const Connection&) = delete;
    Connection& operator=(const Connection&) = delete;

    // Execute ad-hoc SQL
    klyro::Result<api::Result> execute(std::string_view sql);
    
    // Execute SQL with parameters
    klyro::Result<api::Result> execute(std::string_view sql, std::span<const types::Value> parameters);

    // Prepare a statement for reuse
    klyro::Result<PreparedStatement> prepare(std::string_view sql);

    // Transaction management
    klyro::Result<Transaction> begin();
    klyro::Result<Transaction> transaction(); // RAII guard
    
    klyro::Result<void> commit();
    klyro::Result<void> rollback();

    // Set autocommit mode
    void set_autocommit(bool enabled);

    // Cancel currently executing query
    void cancel();
    
    // Close connection manually (also happens on destruction)
    void close();

private:
    friend class klyro::Database;
    explicit Connection(std::unique_ptr<ConnectionImpl> impl);
    
    std::unique_ptr<ConnectionImpl> m_impl;
};

} // namespace klyro::api

#endif // KLYRO_API_CONNECTION_HPP
