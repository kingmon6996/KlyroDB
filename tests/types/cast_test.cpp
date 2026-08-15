#include <gtest/gtest.h>
#include "klyro/types/value.hpp"
#include "klyro/types/type_cast.hpp"

using namespace klyro::types;

TEST(TypeCastTest, ImplicitNumericCasts) {
    Value i(static_cast<std::int32_t>(42));
    
    auto bi = TypeCast::cast(i, TypeID::BigInt);
    ASSERT_TRUE(bi.has_value());
    EXPECT_EQ(bi->type(), TypeID::BigInt);
    EXPECT_EQ(bi->get<std::int64_t>(), 42LL);
    
    auto d = TypeCast::cast(i, TypeID::Double);
    ASSERT_TRUE(d.has_value());
    EXPECT_EQ(d->type(), TypeID::Double);
    EXPECT_DOUBLE_EQ(d->get<double>(), 42.0);
}

TEST(TypeCastTest, OverflowCast) {
    Value bi(static_cast<std::int64_t>(10000000000LL)); // Exceeds 32-bit int
    
    auto i = TypeCast::cast(bi, TypeID::Integer);
    EXPECT_FALSE(i.has_value()); // Should fail overflow check
    
    Value bi2(static_cast<std::int64_t>(42LL));
    auto i2 = TypeCast::cast(bi2, TypeID::Integer);
    ASSERT_TRUE(i2.has_value());
    EXPECT_EQ(i2->get<std::int32_t>(), 42);
}

TEST(TypeCastTest, StringParsingCasts) {
    Value s1("12345", TypeID::Text);
    
    auto i = TypeCast::cast(s1, TypeID::Integer);
    ASSERT_TRUE(i.has_value());
    EXPECT_EQ(i->get<std::int32_t>(), 12345);
    
    Value s2("550e8400-e29b-41d4-a716-446655440000", TypeID::Text);
    auto u = TypeCast::cast(s2, TypeID::UUID);
    ASSERT_TRUE(u.has_value());
    EXPECT_EQ(u->type(), TypeID::UUID);
}

TEST(TypeCastTest, StringInvalidParse) {
    Value s("not an integer", TypeID::Text);
    
    auto i = TypeCast::cast(s, TypeID::Integer);
    EXPECT_FALSE(i.has_value()); // Parse fails
}

TEST(TypeCastTest, ToStringCast) {
    Value i(static_cast<std::int32_t>(42));
    
    auto s = TypeCast::cast(i, TypeID::Text);
    ASSERT_TRUE(s.has_value());
    EXPECT_EQ(s->type(), TypeID::Text);
    EXPECT_EQ(s->get<std::string>(), "42");
}
