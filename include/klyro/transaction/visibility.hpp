#ifndef KLYRO_TRANSACTION_VISIBILITY_HPP
#define KLYRO_TRANSACTION_VISIBILITY_HPP

#include "klyro/transaction/snapshot.hpp"
#include "klyro/transaction/mvcc_header.hpp"
#include "klyro/transaction/transaction_registry.hpp"

namespace klyro::transaction {

class VisibilityManager {
public:
    explicit VisibilityManager(TransactionRegistry& registry);
    
    // Core MVCC visibility rule evaluating if a physical version is logically visible
    // to the given snapshot and current transaction context.
    bool is_visible(const MVCCHeader& header, const Snapshot& snapshot, TransactionID current_txn) const;
    
private:
    TransactionRegistry& m_registry;
};

} // namespace klyro::transaction

#endif // KLYRO_TRANSACTION_VISIBILITY_HPP
