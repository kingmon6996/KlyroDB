#ifndef KLYRO_STORAGE_RECORD_DESERIALIZER_HPP
#define KLYRO_STORAGE_RECORD_DESERIALIZER_HPP

#include "klyro/storage/record.hpp"
#include "klyro/storage/record_view.hpp"
#include <span>
#include <cstddef>

namespace klyro::storage {

class RecordDeserializer {
public:
    // Deserialize a full record (materializes all Values)
    static Record deserialize(std::span<const std::byte> bytes, const TupleLayout& layout);
};

} // namespace klyro::storage

#endif // KLYRO_STORAGE_RECORD_DESERIALIZER_HPP
