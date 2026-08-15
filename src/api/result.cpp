#include "klyro/api/result.hpp"
#include "klyro/api/row.hpp"

namespace klyro::api {

class ResultImpl {
public:
    std::size_t affected_rows{0};
    bool has_rows{false};
    std::vector<ColumnMetadata> columns;
    QueryStatistics stats;
};

Result::Result(std::unique_ptr<ResultImpl> impl) : m_impl(std::move(impl)) {}
Result::~Result() = default;
Result::Result(Result&&) noexcept = default;
Result& Result::operator=(Result&&) noexcept = default;

bool Result::has_rows() const noexcept {
    return m_impl ? m_impl->has_rows : false;
}

std::size_t Result::affected_rows() const noexcept {
    return m_impl ? m_impl->affected_rows : 0;
}

std::unique_ptr<Row> Result::next() {
    return nullptr;
}

std::vector<ColumnMetadata> Result::columns() const {
    return m_impl ? m_impl->columns : std::vector<ColumnMetadata>{};
}

QueryStatistics Result::statistics() const {
    return m_impl ? m_impl->stats : QueryStatistics{};
}

} // namespace klyro::api
