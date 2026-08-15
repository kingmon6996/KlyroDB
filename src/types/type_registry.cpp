#include "klyro/types/type_registry.hpp"
#include <unordered_map>
#include <string>
#include <algorithm>
#include <cctype>

namespace klyro::types {

namespace {

// Thread-safe initialized map of aliases to TypeIDs.
const std::unordered_map<std::string, TypeID>& get_alias_map() {
    static const std::unordered_map<std::string, TypeID> map = {
        // Null
        {"NULL", TypeID::Null},
        
        // Boolean
        {"BOOLEAN", TypeID::Boolean},
        {"BOOL", TypeID::Boolean},
        
        // Integers
        {"SMALLINT", TypeID::SmallInt},
        {"INT2", TypeID::SmallInt},
        
        {"INTEGER", TypeID::Integer},
        {"INT", TypeID::Integer},
        {"INT4", TypeID::Integer},
        
        {"BIGINT", TypeID::BigInt},
        {"INT8", TypeID::BigInt},
        
        // Floats
        {"REAL", TypeID::Real},
        {"FLOAT4", TypeID::Real},
        
        {"DOUBLE PRECISION", TypeID::Double},
        {"DOUBLE", TypeID::Double},
        {"FLOAT8", TypeID::Double},
        
        // Exact Numeric
        {"NUMERIC", TypeID::Numeric},
        {"DECIMAL", TypeID::Decimal},
        {"DEC", TypeID::Decimal},
        
        // Strings
        {"CHARACTER", TypeID::Char},
        {"CHAR", TypeID::Char},
        
        {"CHARACTER VARYING", TypeID::VarChar},
        {"VARCHAR", TypeID::VarChar},
        
        {"TEXT", TypeID::Text},
        
        // Binary
        {"BYTEA", TypeID::Bytea},
        {"BLOB", TypeID::Bytea}, // common alias
        
        // Temporal
        {"DATE", TypeID::Date},
        {"TIME", TypeID::Time},
        {"TIMESTAMP", TypeID::Timestamp},
        {"TIMESTAMPTZ", TypeID::TimestampTZ},
        {"INTERVAL", TypeID::Interval},
        
        // UUID
        {"UUID", TypeID::UUID},
        
        // JSON
        {"JSON", TypeID::JSON},
        {"DICT", TypeID::DICT},
        {"DICTIONARY", TypeID::DICT},
        {"ARRAY", TypeID::Array}
    };
    return map;
}

std::string to_upper(std::string_view sv) {
    std::string result(sv);
    std::transform(result.begin(), result.end(), result.begin(), [](unsigned char c) {
        return std::toupper(c);
    });
    return result;
}

} // namespace

std::optional<TypeID> TypeRegistry::lookup_type(std::string_view name) noexcept {
    auto upper_name = to_upper(name);
    const auto& map = get_alias_map();
    
    auto it = map.find(upper_name);
    if (it != map.end()) {
        return it->second;
    }
    return std::nullopt;
}

bool TypeRegistry::is_valid_alias(std::string_view name) noexcept {
    return lookup_type(name).has_value();
}

} // namespace klyro::types
