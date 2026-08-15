#ifndef KLYRO_INDEX_INDEX_COMPARATOR_HPP
#define KLYRO_INDEX_INDEX_COMPARATOR_HPP

#include "klyro/index/index_key.hpp"

namespace klyro::index {

class IndexComparator {
public:
    // Compares two index keys lexicographically.
    // Returns:
    //  < 0 if lhs < rhs
    // == 0 if lhs == rhs
    //  > 0 if lhs > rhs
    //
    // Nulls are treated as strictly smaller than non-nulls (NULLS FIRST).
    static int compare(const IndexKey& lhs, const IndexKey& rhs) noexcept;
};

} // namespace klyro::index

#endif // KLYRO_INDEX_INDEX_COMPARATOR_HPP
