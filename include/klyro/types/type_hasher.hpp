#ifndef KLYRO_TYPES_TYPE_HASHER_HPP
#define KLYRO_TYPES_TYPE_HASHER_HPP

#include "klyro/types/value.hpp"
#include <cstddef>

namespace klyro::types {

class TypeHasher {
public:
    // Computes a deterministic hash for a Value.
    static std::size_t hash(const Value& value) noexcept;
};

} // namespace klyro::types

#endif // KLYRO_TYPES_TYPE_HASHER_HPP
