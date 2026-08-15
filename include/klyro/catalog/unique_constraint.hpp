#ifndef KLYRO_CATALOG_UNIQUE_CONSTRAINT_HPP
#define KLYRO_CATALOG_UNIQUE_CONSTRAINT_HPP

#include "klyro/catalog/constraint.hpp"
#include <vector>

namespace klyro::catalog {

class UniqueConstraint : public Constraint {
public:
    UniqueConstraint(ConstraintID id, TableID table_id, std::string name, 
                     std::vector<ColumnID> columns, IndexID index_id)
        : Constraint(id, table_id, std::move(name), ConstraintType::Unique),
          m_columns(std::move(columns)), m_index_id(index_id) {}

    const std::vector<ColumnID>& columns() const { return m_columns; }
    IndexID index_id() const { return m_index_id; }

private:
    std::vector<ColumnID> m_columns;
    IndexID m_index_id; // B+ Tree backing this unique constraint
};

} // namespace klyro::catalog

#endif // KLYRO_CATALOG_UNIQUE_CONSTRAINT_HPP
