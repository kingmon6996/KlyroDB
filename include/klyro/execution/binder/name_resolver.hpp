#ifndef KLYRO_EXECUTION_BINDER_NAME_RESOLVER_HPP
#define KLYRO_EXECUTION_BINDER_NAME_RESOLVER_HPP

#include "klyro/catalog/catalog.hpp"
#include "klyro/execution/binder/bound_expression.hpp"
#include <string>
#include <unordered_map>
#include <vector>

namespace klyro::execution::binder {

// Maintains scope and aliases for tables and columns during binding
class NameResolver {
public:
    NameResolver(catalog::Catalog* catalog) : m_catalog(catalog) {}

    // Register a table in the current scope
    Status add_table(const std::string& table_name, const std::string& alias = "");

    // Resolve a column name (optionally qualified) to a table and column ID
    Result<BoundColumnRef> resolve_column(const std::string& col_name, const std::string& table_alias = "");

private:
    catalog::Catalog* m_catalog;
    
    struct BoundTableContext {
        TableID table_id;
        std::string table_name;
        std::string alias;
        catalog::TableMetadata metadata;
    };
    
    std::vector<BoundTableContext> m_active_tables;
};

} // namespace klyro::execution::binder

#endif // KLYRO_EXECUTION_BINDER_NAME_RESOLVER_HPP
