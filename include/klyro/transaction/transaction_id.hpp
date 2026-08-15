#ifndef KLYRO_TRANSACTION_TRANSACTION_ID_HPP
#define KLYRO_TRANSACTION_TRANSACTION_ID_HPP

#include <cstdint>
#include <functional>

namespace klyro::transaction {

using TransactionID = std::uint64_t;

constexpr TransactionID INVALID_TRANSACTION_ID = 0;
constexpr TransactionID INITIAL_TRANSACTION_ID = 1;

} // namespace klyro::transaction

#endif // KLYRO_TRANSACTION_TRANSACTION_ID_HPP
