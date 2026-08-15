#ifndef KLYRO_TYPES_TYPE_CAST_HPP
#define KLYRO_TYPES_TYPE_CAST_HPP

#include "klyro/types/value.hpp"
#include <optional>

namespace klyro::types {

class TypeCast {
public:
    // Casts a value to a target type.
    // Returns nullopt if the cast is invalid or overflows.
    static std::optional<Value> cast(const Value& value, TypeID target_type);
};

} // namespace klyro::types

#endif // KLYRO_TYPES_TYPE_CAST_HPP
