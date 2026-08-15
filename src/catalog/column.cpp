#include "klyro/catalog/column.hpp"

namespace klyro::catalog {

Column::Column(ColumnID id, std::string name, types::TypeID type, std::uint32_t ordinal, bool nullable)
    : m_id(id), m_name(std::move(name)), m_type(type), m_ordinal(ordinal), m_nullable(nullable) {}

} // namespace klyro::catalog
