#ifndef KLYRO_TYPES_TYPE_REGISTRY_HPP
#define KLYRO_TYPES_TYPE_REGISTRY_HPP

#include "klyro/types/type_info.hpp"
#include <string_view>
#include <optional>

namespace klyro::types {

class TypeRegistry {
public:
    // Case-insensitive lookup of type name or alias.
    // E.g., "INT" -> TypeID::Integer, "CHARACTER VARYING" -> TypeID::VarChar
    static std::optional<TypeID> lookup_type(std::string_view name) noexcept;
    
    // Check if alias is valid
    static bool is_valid_alias(std::string_view name) noexcept;
};

} // namespace klyro::types

#endif // KLYRO_TYPES_TYPE_REGISTRY_HPP
