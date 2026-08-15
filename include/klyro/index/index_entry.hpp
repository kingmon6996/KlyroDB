#ifndef KLYRO_INDEX_INDEX_ENTRY_HPP
#define KLYRO_INDEX_INDEX_ENTRY_HPP

#include "klyro/index/index_key.hpp"
#include "klyro/storage/record_id.hpp"
#include "klyro/core/ids.hpp"

namespace klyro::index {

// An entry inside a Leaf Node: (Key -> RecordID)
struct LeafEntry {
    IndexKey key;
    storage::RecordID record_id;
};

// An entry inside an Internal Node: (Separator Key -> PageID)
struct InternalEntry {
    IndexKey key;
    PageID child_page_id;
};

} // namespace klyro::index

#endif // KLYRO_INDEX_INDEX_ENTRY_HPP
