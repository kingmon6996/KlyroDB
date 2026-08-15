#ifndef KLYRO_TYPES_TYPE_SERIALIZER_HPP
#define KLYRO_TYPES_TYPE_SERIALIZER_HPP

#include "klyro/types/value.hpp"
#include <vector>
#include <cstddef>

namespace klyro::types {

class TypeSerializer {
public:
    // Serializes a value into a deterministic byte format.
    // Handles endianness explicitely (Little-Endian).
    static std::vector<std::uint8_t> serialize(const Value& value);
    
    // Deserializes a value from bytes, assuming the caller knows the target TypeID.
    static Value deserialize(const std::vector<std::uint8_t>& bytes, TypeID type);
};

} // namespace klyro::types

#endif // KLYRO_TYPES_TYPE_SERIALIZER_HPP
