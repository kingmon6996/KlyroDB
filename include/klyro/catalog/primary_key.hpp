#ifndef KLYRO_CATALOG_PRIMARY_KEY_HPP
#define KLYRO_CATALOG_PRIMARY_KEY_HPP

#include "klyro/catalog/constraint.hpp"
#include <vector>

namespace klyro::catalog {

class PrimaryKeyConstraint : public Constraint {
public:
    PrimaryKeyConstraint(ConstraintID id, TableID table_id, std::string name, 
                         std::vector<ColumnID> columns, IndexID index_id)
        : Constraint(id, table_id, std::move(name), ConstraintType::PrimaryKey),
          m_columns(std::move(columns)), m_index_id(index_id) {}

    const std::vector<ColumnID>& columns() const { return m_columns; }
    IndexID index_id() const { return m_index_id; }

private:
    std::vector<ColumnID> m_columns;
    IndexID m_index_id; // B+ Tree backing this PK
};

} // namespace klyro::catalog

#endif // KLYRO_CATALOG_PRIMARY_KEY_HPP
