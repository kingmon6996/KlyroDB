#ifndef KLYRO_API_PREPARED_STATEMENT_HPP
#define KLYRO_API_PREPARED_STATEMENT_HPP

#include "klyro/core/result.hpp"
#include "klyro/api/result.hpp"
#include "klyro/types/value.hpp"
#include <memory>
#include <string_view>
#include <vector>

namespace klyro::api {

class PreparedStatementImpl;

class PreparedStatement {
public:
    PreparedStatement(PreparedStatement&&) noexcept;
    PreparedStatement& operator=(PreparedStatement&&) noexcept;
    ~PreparedStatement();

    // Bind by 1-based index
    klyro::Result<void> bind(std::size_t index, const types::Value& value);
    
    // Bind by name (e.g. ":user_id")
    klyro::Result<void> bind(std::string_view name, const types::Value& value);

    klyro::Result<api::Result> execute();

private:
    friend class Connection;
    explicit PreparedStatement(std::unique_ptr<PreparedStatementImpl> impl);
    
    std::unique_ptr<PreparedStatementImpl> m_impl;
};

} // namespace klyro::api

#endif // KLYRO_API_PREPARED_STATEMENT_HPP
