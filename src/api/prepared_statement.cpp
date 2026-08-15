#include "klyro/api/prepared_statement.hpp"

namespace klyro::api {

class PreparedStatementImpl {
public:
    std::string sql;
    std::vector<types::Value> params;
};

PreparedStatement::PreparedStatement(std::unique_ptr<PreparedStatementImpl> impl) 
    : m_impl(std::move(impl)) {}
    
PreparedStatement::~PreparedStatement() = default;
PreparedStatement::PreparedStatement(PreparedStatement&&) noexcept = default;
PreparedStatement& PreparedStatement::operator=(PreparedStatement&&) noexcept = default;

Result<void> PreparedStatement::bind(std::size_t index, const types::Value& value) {
    if (!m_impl) return Status::InvalidState;
    if (index == 0) return Status::InvalidArgument; // 1-based
    if (index > m_impl->params.size()) {
        m_impl->params.resize(index);
    }
    m_impl->params[index - 1] = value;
    return Status::Success;
}

Result<void> PreparedStatement::bind(std::string_view name, const types::Value& value) {
    return Status::Unsupported;
}

Result<Result> PreparedStatement::execute() {
    return Status::Unsupported;
}

} // namespace klyro::api
