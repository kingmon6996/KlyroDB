#ifndef KLYRO_CORE_RESULT_HPP
#define KLYRO_CORE_RESULT_HPP

#include "klyro/core/status.hpp"
#include <expected>
#include <type_traits>

namespace klyro {

// We use std::expected for our Result abstraction since C++23, but it's often available in C++20 compilers
// or as an extension. If std::expected isn't available, we could use a custom implementation.
// Assuming std::expected is available with MSVC/GCC/Clang in C++20/C++23 mode.
// We'll wrap std::expected to provide the Result API.

// Represents a successful result or a failure status.
template <typename T>
class Result {
public:
    // Constructors
    Result(T value) : m_expected(std::move(value)) {}
    
    template <typename U, typename std::enable_if_t<std::is_constructible_v<T, U>, int> = 0>
    Result(U&& value) : m_expected(std::forward<U>(value)) {}
    Result(Status status) : m_expected(std::unexpected(status)) {}

    // Implicit boolean conversion to check for success
    explicit operator bool() const noexcept {
        return m_expected.has_value();
    }

    // Accessors
    T& value() & {
        return m_expected.value();
    }

    const T& value() const & {
        return m_expected.value();
    }
    
    T&& value() && {
        return std::move(m_expected).value();
    }

    Status error() const noexcept {
        return m_expected.error();
    }

private:
    std::expected<T, Status> m_expected;
};

// Specialization for void
template <>
class Result<void> {
public:
    Result() : m_expected(std::expected<void, Status>()) {}
    Result(Status status) : m_expected(std::unexpected(status)) {}

    explicit operator bool() const noexcept {
        return m_expected.has_value();
    }

    void value() const {
        m_expected.value();
    }

    Status error() const noexcept {
        return m_expected.error();
    }

    // Helper for success Result<void>
    static Result<void> success() {
        return Result<void>();
    }

private:
    std::expected<void, Status> m_expected;
};

} // namespace klyro

#endif // KLYRO_CORE_RESULT_HPP
