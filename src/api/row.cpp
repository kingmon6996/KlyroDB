#include "klyro/api/row.hpp"
#include "klyro/core/error.hpp"

namespace klyro::api {

class RowImpl {
public:
    std::vector<types::Value> values;
};

Row::Row(std::unique_ptr<RowImpl> impl) : m_impl(std::move(impl)) {}
Row::~Row() = default;
Row::Row(Row&&) noexcept = default;
Row& Row::operator=(Row&&) noexcept = default;

types::Value Row::get(std::size_t index) const {
    if (!m_impl || index >= m_impl->values.size()) {
        return types::Value(); // Null
    }
    return m_impl->values[index];
}

types::Value Row::get(std::string_view name) const {
    return types::Value();
}

std::size_t Row::size() const noexcept {
    return m_impl ? m_impl->values.size() : 0;
}

} // namespace klyro::api
