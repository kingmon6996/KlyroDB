#ifndef KLYRO_CATALOG_CATALOG_ID_HPP
#define KLYRO_CATALOG_CATALOG_ID_HPP

#include "klyro/core/ids.hpp"
#include "klyro/core/types.hpp"

namespace klyro::catalog {

using SchemaID = StrongID<struct SchemaIDTag, std::uint32_t>;
using TableID = klyro::TableID;
using ColumnID = klyro::core::ColumnID;
using IndexID = klyro::IndexID;
using ConstraintID = StrongID<struct ConstraintIDTag, std::uint32_t>;

} // namespace klyro::catalog

#endif // KLYRO_CATALOG_CATALOG_ID_HPP
