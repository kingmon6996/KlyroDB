#include "klyro/storage/tuple_layout.hpp"
#include "klyro/types/type_info.hpp"

namespace klyro::storage {

void TupleLayout::add_column(types::TypeID type) {
    const auto& info = types::get_type_info(type);
    
    Column col;
    col.type = type;
    
    if (info.is_variable_length) {
        col.offset = 0;
        col.fixed_length = 0;
        m_variable_count++;
    } else {
        col.offset = m_fixed_size;
        col.fixed_length = static_cast<std::uint32_t>(info.fixed_size);
        m_fixed_size += col.fixed_length;
    }
    
    m_columns.push_back(col);
}

} // namespace klyro::storage
