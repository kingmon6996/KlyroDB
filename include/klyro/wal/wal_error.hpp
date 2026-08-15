#ifndef KLYRO_WAL_WAL_ERROR_HPP
#define KLYRO_WAL_WAL_ERROR_HPP

#include <stdexcept>
#include <string>

namespace klyro::wal {

class WALError : public std::runtime_error {
public:
    explicit WALError(const std::string& message) : std::runtime_error(message) {}
};

class WALCorruptionError : public WALError {
public:
    explicit WALCorruptionError(const std::string& message) : WALError(message) {}
};

class UnsupportedWALVersionError : public WALError {
public:
    explicit UnsupportedWALVersionError(const std::string& message) : WALError(message) {}
};

} // namespace klyro::wal

#endif // KLYRO_WAL_WAL_ERROR_HPP
