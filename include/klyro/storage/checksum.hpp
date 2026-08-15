#ifndef KLYRO_STORAGE_CHECKSUM_HPP
#define KLYRO_STORAGE_CHECKSUM_HPP

#include <cstddef>
#include <cstdint>
#include <span>

namespace klyro::storage {

// Deterministic CRC32C checksum function.
// Used for checksumming pages and database headers.
std::uint32_t calculate_checksum(std::span<const std::byte> data) noexcept;

} // namespace klyro::storage

#endif // KLYRO_STORAGE_CHECKSUM_HPP
