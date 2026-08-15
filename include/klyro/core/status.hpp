#ifndef KLYRO_CORE_STATUS_HPP
#define KLYRO_CORE_STATUS_HPP

#include <string_view>

namespace klyro {

// Expected database statuses
enum class Status {
    OK,
    InvalidArgument,
    NotFound,
    AlreadyExists,
    InvalidState,
    IOError,
    Corruption,
    Busy,
    Timeout,
    TransactionAborted,
    Unsupported,
    InternalError
};

// Convert a Status to a human-readable string
std::string_view to_string(Status status) noexcept;

} // namespace klyro

#endif // KLYRO_CORE_STATUS_HPP
