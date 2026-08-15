#ifndef KLYRO_CORE_ERROR_HPP
#define KLYRO_CORE_ERROR_HPP

#include <stdexcept>
#include <string>

namespace klyro {

// Base class for all internal exceptions in KlyroDB.
// Expected database failures (e.g., duplicate key) should use Result<T> and Status.
// Exceptions are reserved for exceptional or unrecoverable internal conditions.
class KlyroException : public std::runtime_error {
public:
    explicit KlyroException(const std::string& message)
        : std::runtime_error(message) {}

    explicit KlyroException(const char* message)
        : std::runtime_error(message) {}
};

// Specialized exceptions can be added here if needed in the future.

} // namespace klyro

#endif // KLYRO_CORE_ERROR_HPP
