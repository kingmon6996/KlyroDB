#ifndef KLYRO_SQL_SQL_ERROR_HPP
#define KLYRO_SQL_SQL_ERROR_HPP

#include "klyro/core/status.hpp"
#include "klyro/sql/source_location.hpp"
#include <string>

namespace klyro::sql {

class SQLError {
public:
    static Status make_error(Status base_status, const SourceLocation& loc, const std::string& message);
};

} // namespace klyro::sql

#endif // KLYRO_SQL_SQL_ERROR_HPP
