#ifndef KLYRO_SQL_SOURCE_LOCATION_HPP
#define KLYRO_SQL_SOURCE_LOCATION_HPP

#include <cstddef>
#include <string>

namespace klyro::sql {

struct SourceLocation {
    std::size_t offset{0};
    std::size_t line{1};
    std::size_t column{1};

    std::string to_string() const;
};

} // namespace klyro::sql

#endif // KLYRO_SQL_SOURCE_LOCATION_HPP
