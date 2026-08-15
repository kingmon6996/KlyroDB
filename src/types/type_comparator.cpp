#include "klyro/types/type_comparator.hpp"

namespace klyro::types {

bool TypeComparator::equal(const Value& lhs, const Value& rhs) noexcept {
    return lhs == rhs; // Value class handles null-safe equality and type matching
}

bool TypeComparator::less(const Value& lhs, const Value& rhs) noexcept {
    return lhs < rhs; // Value class handles nulls-first logic
}

} // namespace klyro::types
