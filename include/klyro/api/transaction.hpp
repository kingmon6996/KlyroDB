#ifndef KLYRO_API_TRANSACTION_HPP
#define KLYRO_API_TRANSACTION_HPP

#include "klyro/core/result.hpp"
#include <memory>
#include <string_view>

namespace klyro::api {

class Connection;

class Transaction {
public:
    // Destructor rolls back if not explicitly committed
    ~Transaction();

    Transaction(Transaction&&) noexcept;
    Transaction& operator=(Transaction&&) noexcept;
    
    // Non-copyable
    Transaction(const Transaction&) = delete;
    Transaction& operator=(const Transaction&) = delete;

    klyro::Result<void> commit();
    klyro::Result<void> rollback();

    klyro::Result<void> savepoint(std::string_view name);
    klyro::Result<void> rollback_to(std::string_view name);
    klyro::Result<void> release_savepoint(std::string_view name);

private:
    friend class Connection;
    explicit Transaction(Connection* conn);
    
    Connection* m_conn;
    bool m_active;
};

} // namespace klyro::api

#endif // KLYRO_API_TRANSACTION_HPP
