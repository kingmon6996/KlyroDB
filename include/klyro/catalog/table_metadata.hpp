#ifndef KLYRO_CATALOG_TABLE_METADATA_HPP
#define KLYRO_CATALOG_TABLE_METADATA_HPP

#include "klyro/catalog/catalog_id.hpp"
#include "klyro/catalog/table_schema.hpp"
#include "klyro/core/ids.hpp"
#include <string>

namespace klyro::catalog {

struct TableMetadata {
    TableID table_id;
    SchemaID schema_id;
    std::string name;
    
    TableSchema schema;
    std::uint32_t version{1};
    
    // The starting page of the TableHeap that actually contains the records
    PageID first_page_id{};
};

} // namespace klyro::catalog

#endif // KLYRO_CATALOG_TABLE_METADATA_HPP
