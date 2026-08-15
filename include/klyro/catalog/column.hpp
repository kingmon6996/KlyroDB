#ifndef KLYRO_CATALOG_COLUMN_HPP
#define KLYRO_CATALOG_COLUMN_HPP

#include "klyro/catalog/catalog_id.hpp"
#include "klyro/catalog/default_value.hpp"
#include "klyro/types/type_id.hpp"
#include <string>
#include <optional>

namespace klyro::catalog {

struct TypeParameters {
    std::optional<std::uint32_t> length;
    std::optional<std::uint32_t> precision;
    std::optional<std::uint32_t> scale;
};

class Column {
public:
    Column(ColumnID id, std::string name, types::TypeID type, std::uint32_t ordinal, bool nullable);

    ColumnID id() const { return m_id; }
    const std::string& name() const { return m_name; }
    types::TypeID type() const { return m_type; }
    std::uint32_t ordinal() const { return m_ordinal; }
    bool is_nullable() const { return m_nullable; }
    
    const TypeParameters& parameters() const { return m_params; }
    void set_parameters(TypeParameters params) { m_params = std::move(params); }
    
    const DefaultValue& default_value() const { return m_default_value; }
    void set_default_value(DefaultValue default_val) { m_default_value = std::move(default_val); }

private:
    ColumnID m_id;
    std::string m_name;
    types::TypeID m_type;
    std::uint32_t m_ordinal;
    bool m_nullable;
    
    TypeParameters m_params;
    DefaultValue m_default_value;
};

} // namespace klyro::catalog

#endif // KLYRO_CATALOG_COLUMN_HPP
