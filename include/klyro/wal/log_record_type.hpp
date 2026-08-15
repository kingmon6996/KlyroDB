#ifndef KLYRO_WAL_LOG_RECORD_TYPE_HPP
#define KLYRO_WAL_LOG_RECORD_TYPE_HPP

#include <cstdint>

namespace klyro::wal {

enum class LogRecordType : std::uint8_t {
    Invalid = 0,
    
    // Transaction state
    TxnBegin = 1,
    TxnCommit = 2,
    TxnAbort = 3,
    TxnEnd = 4,
    
    // Physical Operations
    Insert = 10,
    Update = 11,
    Delete = 12,
    
    // Checkpointing
    CheckpointBegin = 20,
    CheckpointEnd = 21,
    
    // Compensation Log Record (Undo)
    CLR = 30,
    
    // System
    PageAllocate = 40,
    PageFree = 41
};

} // namespace klyro::wal

#endif // KLYRO_WAL_LOG_RECORD_TYPE_HPP
