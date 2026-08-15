#ifndef KLYRO_WAL_LSN_HPP
#define KLYRO_WAL_LSN_HPP

#include <cstdint>
#include <limits>
#include <compare>

namespace klyro::wal {

// Log Sequence Number
class LSN {
public:
    constexpr LSN() noexcept = default;
    constexpr explicit LSN(std::uint64_t val) noexcept : m_val(val) {}

    // Special Constants
    static constexpr LSN invalid() noexcept { return LSN(0); }
    static constexpr LSN max() noexcept { return LSN(std::numeric_limits<std::uint64_t>::max()); }

    constexpr bool is_valid() const noexcept { return m_val != 0; }
    constexpr std::uint64_t value() const noexcept { return m_val; }

    auto operator<=>(const LSN&) const = default;

private:
    std::uint64_t m_val{0}; // 0 represents invalid
};

// Also define PageLSN here for convenience, though they are the same type.
using PageLSN = LSN;

} // namespace klyro::wal

#endif // KLYRO_WAL_LSN_HPP
