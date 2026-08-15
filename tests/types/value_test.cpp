#include <gtest/gtest.h>
#include "klyro/types/value.hpp"
#include "klyro/types/array.hpp"
#include <string>

using namespace klyro::types;

TEST(TypeValueTest, NullValue) {
    Value v1;
    EXPECT_TRUE(v1.is_null());
    EXPECT_EQ(v1.type(), TypeID::Null);

    Value v2(TypeID::Integer);
    EXPECT_TRUE(v2.is_null());
    EXPECT_EQ(v2.type(), TypeID::Integer);

    EXPECT_EQ(v1, v2); // NULLs are considered equal in strict C++ ==
}

TEST(TypeValueTest, Primitives) {
    Value b(true);
    EXPECT_FALSE(b.is_null());
    EXPECT_EQ(b.type(), TypeID::Boolean);
    EXPECT_TRUE(b.get<bool>());

    Value i(static_cast<std::int32_t>(42));
    EXPECT_EQ(i.type(), TypeID::Integer);
    EXPECT_EQ(i.get<std::int32_t>(), 42);

    Value bi(static_cast<std::int64_t>(1000000000000LL));
    EXPECT_EQ(bi.type(), TypeID::BigInt);
    EXPECT_EQ(bi.get<std::int64_t>(), 1000000000000LL);
}

TEST(TypeValueTest, Strings) {
    Value s1("hello", TypeID::Text);
    EXPECT_EQ(s1.type(), TypeID::Text);
    EXPECT_EQ(s1.get<std::string>(), "hello");

    Value s2("hello", TypeID::VarChar);
    EXPECT_EQ(s2.type(), TypeID::VarChar);
    EXPECT_EQ(s2.get<std::string>(), "hello");

    EXPECT_NE(s1, s2); // Different types
}

TEST(TypeValueTest, ValueComparisons) {
    Value i1(static_cast<std::int32_t>(10));
    Value i2(static_cast<std::int32_t>(20));
    Value i3(static_cast<std::int32_t>(10));

    EXPECT_EQ(i1, i3);
    EXPECT_NE(i1, i2);
    EXPECT_LT(i1, i2);
    EXPECT_FALSE(i2 < i1);
}

TEST(TypeValueTest, Collections) {
    Array arr(TypeID::Integer);
    arr.push_back(Value(static_cast<std::int32_t>(10)));
    arr.push_back(Value(static_cast<std::int32_t>(20)));
    Value arr_val(std::move(arr));
    
    EXPECT_EQ(arr_val.type(), TypeID::Array);
    EXPECT_EQ(arr_val.get<Array>().size(), 2);
    
    Dict dict;
    dict.set("key", Value(static_cast<std::int32_t>(30)));
    Value dict_val(std::move(dict));
    
    EXPECT_EQ(dict_val.type(), TypeID::DICT);
    EXPECT_TRUE(dict_val.get<Dict>().contains("key"));
}
