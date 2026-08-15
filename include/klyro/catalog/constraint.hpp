#ifndef KLYRO_CATALOG_CONSTRAINT_HPP
#define KLYRO_CATALOG_CONSTRAINT_HPP

#include "klyro/catalog/catalog_id.hpp"
#include <string>

namespace klyro::catalog {

enum class ConstraintType {
    PrimaryKey,
    Unique,
    ForeignKey,
    Check
};

class Constraint {
public:
    Constraint(ConstraintID id, TableID table_id, std::string name, ConstraintType type);
    virtual ~Constraint() = default;

    ConstraintID id() const { return m_id; }
    TableID table_id() const { return m_table_id; }
    const std::string& name() const { return m_name; }
    ConstraintType type() const { return m_type; }

private:
    ConstraintID m_id;
    TableID m_table_id;
    std::string m_name;
    ConstraintType m_type;
};

} // namespace klyro::catalog

#endif // KLYRO_CATALOG_CONSTRAINT_HPP
