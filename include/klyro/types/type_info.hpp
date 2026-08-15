#ifndef KLYRO_TYPES_TYPE_INFO_HPP
#define KLYRO_TYPES_TYPE_INFO_HPP

#include "klyro/types/type_id.hpp"
#include <string_view>

namespace klyro::types {

struct TypeInfo {
    TypeID id;
    std::string_view canonical_name;
    
    std::size_t fixed_size;
    bool is_variable_length;
    
    bool is_orderable;
    bool is_hashable;
    bool is_indexable;
    bool is_numeric;
    bool is_collection;
    bool is_complex;
};

// Returns static info for a given type ID.
const TypeInfo& get_type_info(TypeID id) noexcept;

} // namespace klyro::types

#endif // KLYRO_TYPES_TYPE_INFO_HPP
