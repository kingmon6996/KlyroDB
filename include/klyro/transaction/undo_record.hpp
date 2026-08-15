#ifndef KLYRO_TRANSACTION_UNDO_RECORD_HPP
#define KLYRO_TRANSACTION_UNDO_RECORD_HPP

#include "klyro/storage/record_id.hpp"
#include "klyro/core/ids.hpp"

namespace klyro::transaction {

enum class UndoOperation {
    Insert,
    Update,
    Delete
};

struct UndoRecord {
    UndoOperation op;
    TableID table_id;
    storage::RecordID record_id;
    
    // In a full implementation, we might store the previous version physical bytes
    // inline here or reference it logically, but for now we'll just track the record ID
    // that needs rolling back (e.g. setting its xmax back to invalid, or deleting the inserted row).
};

} // namespace klyro::transaction

#endif // KLYRO_TRANSACTION_UNDO_RECORD_HPP
