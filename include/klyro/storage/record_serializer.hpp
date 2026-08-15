#ifndef KLYRO_STORAGE_RECORD_SERIALIZER_HPP
#define KLYRO_STORAGE_RECORD_SERIALIZER_HPP

#include "klyro/storage/record.hpp"
#include "klyro/storage/tuple_layout.hpp"
#include <vector>
#include <cstddef>

namespace klyro::storage {

class RecordSerializer {
public:
    // Serialize a full record according to a specified tuple layout.
    static std::vector<std::byte> serialize(const Record& record, const TupleLayout& layout);
};

} // namespace klyro::storage

#endif // KLYRO_STORAGE_RECORD_SERIALIZER_HPP
