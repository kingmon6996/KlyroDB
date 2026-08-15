#include <gtest/gtest.h>
#include "klyro/storage/record_serializer.hpp"
#include "klyro/storage/record_deserializer.hpp"

using namespace klyro::storage;
using namespace klyro::types;

TEST(RecordSerializationTest, SimpleFixedLength) {
    TupleLayout layout;
    layout.add_column(TypeID::Integer);
    layout.add_column(TypeID::BigInt);
    layout.add_column(TypeID::Boolean);

    std::vector<Value> fields;
    fields.push_back(Value(static_cast<std::int32_t>(42)));
    fields.push_back(Value(static_cast<std::int64_t>(10000000000LL)));
    fields.push_back(Value(true));

    Record rec(std::move(fields));
    auto bytes = RecordSerializer::serialize(rec, layout);

    Record deserialized = RecordDeserializer::deserialize(bytes, layout);
    EXPECT_EQ(rec, deserialized);
}

TEST(RecordSerializationTest, WithNulls) {
    TupleLayout layout;
    layout.add_column(TypeID::Integer);
    layout.add_column(TypeID::VarChar);
    layout.add_column(TypeID::Integer);

    std::vector<Value> fields;
    fields.push_back(Value(static_cast<std::int32_t>(1)));
    fields.push_back(Value(TypeID::VarChar)); // NULL
    fields.push_back(Value(static_cast<std::int32_t>(3)));

    Record rec(std::move(fields));
    auto bytes = RecordSerializer::serialize(rec, layout);

    Record deserialized = RecordDeserializer::deserialize(bytes, layout);
    EXPECT_EQ(rec, deserialized);
    EXPECT_TRUE(deserialized.field(1).is_null());
}

TEST(RecordSerializationTest, VariableLengthStrings) {
    TupleLayout layout;
    layout.add_column(TypeID::Integer);
    layout.add_column(TypeID::Text);
    layout.add_column(TypeID::Text);

    std::vector<Value> fields;
    fields.push_back(Value(static_cast<std::int32_t>(123)));
    fields.push_back(Value("Hello", TypeID::Text));
    fields.push_back(Value("World! Testing a longer string.", TypeID::Text));

    Record rec(std::move(fields));
    auto bytes = RecordSerializer::serialize(rec, layout);

    Record deserialized = RecordDeserializer::deserialize(bytes, layout);
    EXPECT_EQ(rec, deserialized);
    
    // Test View
    RecordView view(bytes, layout);
    EXPECT_EQ(view.field(1).get<std::string>(), "Hello");
    EXPECT_EQ(view.field_view(2).get_string_view(), "World! Testing a longer string.");
}
