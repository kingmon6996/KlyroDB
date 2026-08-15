#ifndef KLYRO_INDEX_INDEX_METADATA_HPP
#define KLYRO_INDEX_INDEX_METADATA_HPP

#include "klyro/core/ids.hpp"
#include "klyro/types/type_id.hpp"
#include <vector>
#include <string>

namespace klyro::index {

struct IndexMetadata {
    std::uint32_t index_id;
    std::string name;
    PageID root_page_id;
    std::uint32_t tree_height;
    bool is_unique;
    std::vector<types::TypeID> key_types;
};

} // namespace klyro::index

#endif // KLYRO_INDEX_INDEX_METADATA_HPP
