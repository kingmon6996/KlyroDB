#ifndef KLYRO_TRANSACTION_TRANSACTION_MANAGER_HPP
#define KLYRO_TRANSACTION_TRANSACTION_MANAGER_HPP

#include "klyro/transaction/transaction.hpp"
#include "klyro/transaction/transaction_registry.hpp"
#include "klyro/transaction/snapshot.hpp"
#include "klyro/core/status.hpp"
#include <memory>
#include <atomic>
#include <unordered_map>
#include <shared_mutex>

namespace klyro::concurrency { class LockManager; }
namespace klyro::wal { class WALManager; }

namespace klyro::transaction {

class TransactionManager {
public:
    explicit TransactionManager(concurrency::LockManager* lock_manager = nullptr, wal::WALManager* wal_manager = nullptr);
    
    // Begin a new transaction
    Transaction* begin(IsolationLevel iso_level = IsolationLevel::ReadCommitted);
    
    // Commit the transaction
    Status commit(Transaction* txn);
    
    // Abort the transaction
    Status abort(Transaction* txn);
    
    // Create a snapshot for MVCC read operations
    Snapshot create_snapshot(const Transaction* txn);
    
    // Expose the registry to other subsystems (like VisibilityManager)
    TransactionRegistry& get_registry() { return m_registry; }
    
private:
    std::atomic<TransactionID> m_next_txn_id{INITIAL_TRANSACTION_ID};
    
    TransactionRegistry m_registry;
    
    // For Module 9 we keep actual Transaction objects in-memory here for simplicity.
    // In a real system, the client or context might own them, but a manager often tracks them.
    std::unordered_map<TransactionID, std::unique_ptr<Transaction>> m_transactions;
    std::shared_mutex m_txn_mutex;
    
    concurrency::LockManager* m_lock_manager{nullptr};
    wal::WALManager* m_wal_manager{nullptr};
    
    // Rollback logic helper
    void rollback_undo_records(Transaction* txn);
};

} // namespace klyro::transaction

#endif // KLYRO_TRANSACTION_TRANSACTION_MANAGER_HPP
