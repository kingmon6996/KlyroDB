#ifndef KLYRO_TRANSACTION_MVCC_HEADER_HPP
#define KLYRO_TRANSACTION_MVCC_HEADER_HPP

#include "klyro/transaction/transaction_id.hpp"
#include "klyro/storage/record_id.hpp"

namespace klyro::transaction {

// Represents the MVCC metadata prepended/attached to each physical record.
// If xmax is INVALID_TRANSACTION_ID, the row is not deleted/updated.
// prev_version points to the old version in an Old-to-New chaining model.
struct MVCCHeader {
    TransactionID xmin{INVALID_TRANSACTION_ID};
    TransactionID xmax{INVALID_TRANSACTION_ID};
    storage::RecordID prev_version{};
};

} // namespace klyro::transaction

#endif // KLYRO_TRANSACTION_MVCC_HEADER_HPP
