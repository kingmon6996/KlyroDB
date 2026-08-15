#ifndef KLYRO_CATALOG_INDEX_METADATA_HPP
#define KLYRO_CATALOG_INDEX_METADATA_HPP

#include "klyro/catalog/catalog_id.hpp"
#include "klyro/core/ids.hpp"
#include <string>
#include <vector>

namespace klyro::catalog {

enum class IndexType {
    BPlusTree
};

struct IndexMetadata {
    IndexID index_id;
    SchemaID schema_id;
    TableID table_id;
    
    std::string name;
    std::vector<ColumnID> indexed_columns;
    
    // Root page of the B+ Tree
    PageID root_page_id{};
    std::uint32_t tree_height{0};
    
    bool is_unique{false};
    bool is_primary_key{false};
    IndexType type{IndexType::BPlusTree};
};

} // namespace klyro::catalog

#endif // KLYRO_CATALOG_INDEX_METADATA_HPP
