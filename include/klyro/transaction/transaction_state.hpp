#ifndef KLYRO_TRANSACTION_TRANSACTION_STATE_HPP
#define KLYRO_TRANSACTION_TRANSACTION_STATE_HPP

#include <string_view>

namespace klyro::transaction {

enum class TransactionState {
    Active,
    Committing,
    Committed,
    Aborting,
    Aborted
};

constexpr std::string_view to_string(TransactionState state) {
    switch (state) {
        case TransactionState::Active: return "Active";
        case TransactionState::Committing: return "Committing";
        case TransactionState::Committed: return "Committed";
        case TransactionState::Aborting: return "Aborting";
        case TransactionState::Aborted: return "Aborted";
        default: return "Unknown";
    }
}

enum class IsolationLevel {
    ReadCommitted,
    RepeatableRead,
    Serializable
};

} // namespace klyro::transaction

#endif // KLYRO_TRANSACTION_TRANSACTION_STATE_HPP
