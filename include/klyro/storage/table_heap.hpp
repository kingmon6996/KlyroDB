#ifndef KLYRO_STORAGE_TABLE_HEAP_HPP
#define KLYRO_STORAGE_TABLE_HEAP_HPP

#include "klyro/storage/table_page.hpp"
#include "klyro/storage/buffer_pool.hpp"
#include "klyro/storage/record_serializer.hpp"
#include "klyro/storage/record_deserializer.hpp"

#include "klyro/transaction/transaction_context.hpp"
#include "klyro/transaction/visibility.hpp"

namespace klyro::wal { class WALManager; }

namespace klyro::storage {

class TableHeap {
public:
    TableHeap(BufferPool* buffer_pool, PageID first_page_id);
    
    // Initialize a new table heap
    static Result<TableHeap> create(BufferPool* buffer_pool);

    Result<RecordID> insert(const Record& record, const TupleLayout& layout, transaction::TransactionContext& ctx);
    
    // Physical raw get - no MVCC logic
    Result<Record> get_physical(const RecordID& id, const TupleLayout& layout);
    
    // MVCC-aware get - resolves visibility through the version chain
    Result<Record> get(const RecordID& id, const TupleLayout& layout, transaction::TransactionContext& ctx, const transaction::VisibilityManager& visibility_mgr);
    
    // MVCC-aware update (physically inserts new version, marks old version xmax)
    Result<void> update(const RecordID& id, const Record& record, const TupleLayout& layout, transaction::TransactionContext& ctx);
    
    // MVCC-aware delete (marks old version xmax)
    Result<void> erase(const RecordID& id, transaction::TransactionContext& ctx);

    PageID first_page_id() const { return m_first_page_id; }
    
    // Inject WAL Manager
    void set_wal_manager(wal::WALManager* wal) { m_wal_manager = wal; }

private:
    BufferPool* m_buffer_pool;
    PageID m_first_page_id;
    wal::WALManager* m_wal_manager{nullptr};
};

} // namespace klyro::storage

#endif // KLYRO_STORAGE_TABLE_HEAP_HPP
