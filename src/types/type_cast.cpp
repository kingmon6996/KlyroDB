#include "klyro/types/type_cast.hpp"
#include <string>
#include <stdexcept>
#include <limits>
#include <cmath>

namespace klyro::types {

std::optional<Value> TypeCast::cast(const Value& value, TypeID target_type) {
    if (value.is_null()) {
        return Value(target_type);
    }
    
    if (value.type() == target_type) {
        return value;
    }
    
    // Simplistic casting logic for V1.
    // Real engines have large conversion matrices.
    
    // Numeric -> Numeric
    if (target_type == TypeID::BigInt) {
        if (value.type() == TypeID::Integer) return Value(static_cast<std::int64_t>(value.get<std::int32_t>()));
        if (value.type() == TypeID::SmallInt) return Value(static_cast<std::int64_t>(value.get<std::int16_t>()));
        if (value.type() == TypeID::Double) return Value(static_cast<std::int64_t>(value.get<double>()));
    }
    
    if (target_type == TypeID::Integer) {
        if (value.type() == TypeID::BigInt) {
            auto v = value.get<std::int64_t>();
            if (v > std::numeric_limits<std::int32_t>::max() || v < std::numeric_limits<std::int32_t>::min()) return std::nullopt; // Overflow
            return Value(static_cast<std::int32_t>(v));
        }
    }
    
    if (target_type == TypeID::Double) {
        if (value.type() == TypeID::Integer) return Value(static_cast<double>(value.get<std::int32_t>()));
        if (value.type() == TypeID::BigInt) return Value(static_cast<double>(value.get<std::int64_t>()));
    }
    
    // String parsing
    if (value.type() == TypeID::Text || value.type() == TypeID::VarChar || value.type() == TypeID::Char) {
        const auto& str = value.get<std::string>();
        try {
            if (target_type == TypeID::Integer) return Value(static_cast<std::int32_t>(std::stoi(str)));
            if (target_type == TypeID::BigInt) return Value(static_cast<std::int64_t>(std::stoll(str)));
            if (target_type == TypeID::Double) return Value(std::stod(str));
            if (target_type == TypeID::UUID) {
                auto u = UUID::from_string(str);
                if (u) return Value(*u);
                return std::nullopt;
            }
        } catch (...) {
            return std::nullopt; // Parse failure
        }
    }
    
    // Cast to String
    if (target_type == TypeID::Text || target_type == TypeID::VarChar || target_type == TypeID::Char) {
        return Value(value.to_string(), target_type);
    }
    
    return std::nullopt;
}

} // namespace klyro::types
