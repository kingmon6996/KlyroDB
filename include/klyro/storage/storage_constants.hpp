#ifndef KLYRO_STORAGE_CONSTANTS_HPP
#define KLYRO_STORAGE_CONSTANTS_HPP

#include <cstdint>
#include <array>
#include <cstddef>

namespace klyro::storage {

// The magic string identifying a KlyroDB database file: "KLYRODB\0"
constexpr std::array<char, 8> MAGIC_BYTES = {'K', 'L', 'Y', 'R', 'O', 'D', 'B', '\0'};

// Database UUID size
constexpr std::size_t UUID_SIZE = 16;

} // namespace klyro::storage

#endif // KLYRO_STORAGE_CONSTANTS_HPP
