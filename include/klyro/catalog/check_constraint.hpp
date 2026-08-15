#ifndef KLYRO_CATALOG_CHECK_CONSTRAINT_HPP
#define KLYRO_CATALOG_CHECK_CONSTRAINT_HPP

#include "klyro/catalog/constraint.hpp"

namespace klyro::catalog {

class CheckConstraint : public Constraint {
public:
    CheckConstraint(ConstraintID id, TableID table_id, std::string name, std::string expression)
        : Constraint(id, table_id, std::move(name), ConstraintType::Check),
          m_expression(std::move(expression)) {}

    const std::string& expression() const { return m_expression; }

private:
    std::string m_expression; // Serialized AST or expression string placeholder
};

} // namespace klyro::catalog

#endif // KLYRO_CATALOG_CHECK_CONSTRAINT_HPP
