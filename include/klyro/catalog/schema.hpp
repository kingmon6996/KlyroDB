#ifndef KLYRO_CATALOG_SCHEMA_HPP
#define KLYRO_CATALOG_SCHEMA_HPP

#include "klyro/catalog/catalog_id.hpp"
#include <string>

namespace klyro::catalog {

struct SchemaMetadata {
    SchemaID schema_id;
    std::string name;
};

} // namespace klyro::catalog

#endif // KLYRO_CATALOG_SCHEMA_HPP
