#ifndef KLYRO_TYPES_TYPE_ID_HPP
#define KLYRO_TYPES_TYPE_ID_HPP

#include <cstdint>

namespace klyro::types {

enum class TypeID : std::uint16_t {
    Null = 0,

    Boolean,

    SmallInt,
    Integer,
    BigInt,

    Real,
    Double,

    Numeric, // Aliased to Decimal
    Decimal = Numeric,

    Char,
    VarChar,
    Text,

    Bytea,

    Date,
    Time,
    Timestamp,
    TimestampTZ,
    Interval,

    UUID,

    JSON,
    DICT,

    Array,
    Enum,

    // Sentinel
    Invalid
};

} // namespace klyro::types

#endif // KLYRO_TYPES_TYPE_ID_HPP
