#include "klyro/sql/source_location.hpp"
#include <sstream>

namespace klyro::sql {

std::string SourceLocation::to_string() const {
    std::stringstream ss;
    ss << "line " << line << ", column " << column;
    return ss.str();
}

} // namespace klyro::sql
