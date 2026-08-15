#include <gtest/gtest.h>
#include "klyro/types/value.hpp"
#include "klyro/types/type_serializer.hpp"

using namespace klyro::types;

TEST(TypeSerializationTest, PrimitivesRoundTrip) {
    auto test_roundtrip = [](const Value& val) {
        auto bytes = TypeSerializer::serialize(val);
        auto deserialized = TypeSerializer::deserialize(bytes, val.type());
        EXPECT_EQ(val, deserialized);
    };

    test_roundtrip(Value(true));
    test_roundtrip(Value(false));
    test_roundtrip(Value(static_cast<std::int16_t>(-123)));
    test_roundtrip(Value(static_cast<std::int32_t>(123456789)));
    test_roundtrip(Value(static_cast<std::int64_t>(-123456789012345LL)));
    test_roundtrip(Value(3.14159f));
    test_roundtrip(Value(2.718281828));
}

TEST(TypeSerializationTest, ComplexRoundTrip) {
    auto test_roundtrip = [](const Value& val) {
        auto bytes = TypeSerializer::serialize(val);
        auto deserialized = TypeSerializer::deserialize(bytes, val.type());
        EXPECT_EQ(val, deserialized);
    };

    test_roundtrip(Value(Decimal(12345, 2)));
    test_roundtrip(Value(std::string("Hello World"), TypeID::Text));
    test_roundtrip(Value(std::vector<std::uint8_t>{0xde, 0xad, 0xbe, 0xef}));
    test_roundtrip(Value(Date(1234)));
    test_roundtrip(Value(Time(56789)));
    test_roundtrip(Value(Timestamp(987654321)));
    test_roundtrip(Value(Interval(1, 2, 3)));
    
    auto uuid_opt = UUID::from_string("550e8400-e29b-41d4-a716-446655440000");
    ASSERT_TRUE(uuid_opt.has_value());
    test_roundtrip(Value(uuid_opt.value()));
    
    test_roundtrip(Value(Json::parse("{\"a\": 1}")));
    
    Array arr(TypeID::Integer);
    arr.push_back(Value(static_cast<std::int32_t>(100)));
    arr.push_back(Value(static_cast<std::int32_t>(200)));
    test_roundtrip(Value(arr));
    
    Dict dict;
    dict.set("key1", Value(std::string("hello"), TypeID::Text));
    dict.set("key2", Value(static_cast<std::int32_t>(42)));
    test_roundtrip(Value(dict));
}

TEST(TypeSerializationTest, NullRoundtrip) {
    Value n1(TypeID::Integer);
    auto b1 = TypeSerializer::serialize(n1);
    EXPECT_TRUE(b1.empty());
    
    // Caller is responsible for knowing it's null, but if passed empty, behavior is undefined/exception.
    // In actual record parsing, null bitmaps handle nulls, they are never passed to deserialize.
}
