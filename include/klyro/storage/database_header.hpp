#ifndef KLYRO_STORAGE_DATABASE_HEADER_HPP
#define KLYRO_STORAGE_DATABASE_HEADER_HPP

#include "klyro/core/ids.hpp"
#include "klyro/storage/storage_constants.hpp"
#include <cstdint>
#include <array>
#include <span>

namespace klyro::storage {

// Represents the parsed database header stored in Page 0.
struct DatabaseHeader {
    std::array<char, 8> magic;
    std::uint32_t format_version;
    std::uint32_t page_size;
    std::array<std::byte, UUID_SIZE> database_id;
    std::uint64_t creation_timestamp;
    PageID catalog_root_page;
    PageID free_page_root;
    PageID next_page_id;
    std::uint64_t flags;
    std::uint32_t checksum;

    // The size of the header when serialized
    static constexpr std::size_t SIZE = 8 + 4 + 4 + 16 + 8 + 8 + 8 + 8 + 8 + 4; // 76 bytes

    void serialize(std::span<std::byte> buffer) const noexcept;
    void deserialize(std::span<const std::byte> buffer) noexcept;
};

} // namespace klyro::storage

#endif // KLYRO_STORAGE_DATABASE_HEADER_HPP
