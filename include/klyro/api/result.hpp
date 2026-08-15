#ifndef KLYRO_API_RESULT_HPP
#define KLYRO_API_RESULT_HPP

#include "klyro/core/result.hpp"
#include "klyro/api/row.hpp"
#include <memory>
#include <vector>
#include <string>

namespace klyro::api {

struct ColumnMetadata {
    std::string name;
    // We would use a DataType enum here. For now, an integer or string representation.
    int type; 
    bool nullable;
    std::string table;
};

struct QueryStatistics {
    double planning_time_ms{0};
    double execution_time_ms{0};
    std::size_t rows_returned{0};
    std::size_t rows_scanned{0};
    std::size_t pages_read{0};
    std::size_t pages_written{0};
    std::size_t buffer_hits{0};
    std::size_t memory_used_bytes{0};
};

class ResultImpl;

class Result {
public:
    Result(Result&&) noexcept;
    Result& operator=(Result&&) noexcept;
    ~Result();

    // Iterator-like access
    bool has_rows() const noexcept;
    std::size_t affected_rows() const noexcept;
    
    // Fetch the next row
    std::unique_ptr<Row> next();
    
    std::vector<ColumnMetadata> columns() const;
    QueryStatistics statistics() const;

private:
    friend class Connection;
    friend class PreparedStatement;
    explicit Result(std::unique_ptr<ResultImpl> impl);
    
    std::unique_ptr<ResultImpl> m_impl;
};

} // namespace klyro::api

#endif // KLYRO_API_RESULT_HPP
