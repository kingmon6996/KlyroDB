#include "klyro/storage/record_deserializer.hpp"

namespace klyro::storage {

Record RecordDeserializer::deserialize(std::span<const std::byte> bytes, const TupleLayout& layout) {
    RecordView view(bytes, layout);
    
    std::vector<types::Value> fields;
    fields.reserve(layout.column_count());
    
    for (std::size_t i = 0; i < layout.column_count(); ++i) {
        fields.push_back(view.field(i));
    }
    
    return Record(std::move(fields));
}

} // namespace klyro::storage
