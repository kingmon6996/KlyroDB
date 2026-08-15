#include "klyro/wal/wal_manager.hpp"
#include "klyro/wal/log_reader.hpp"
#include "klyro/wal/checkpoint_manager.hpp"
#include "klyro/wal/recovery_manager.hpp"
#include "klyro/transaction/transaction_manager.hpp"
#include "klyro/storage/disk_manager.hpp"
#include "klyro/storage/buffer_pool.hpp"
#include "klyro/storage/table_heap.hpp"
#include <iostream>
#include <cassert>
#include <filesystem>

using namespace klyro;
using namespace klyro::wal;

void test_wal_serialization() {
    std::cout << "Running test_wal_serialization...\n";
    
    std::vector<std::uint8_t> payload = {0x01, 0x02, 0x03, 0x04};
    LogRecord rec1(100, LogRecordType::Insert, LSN(10), LSN::invalid(), payload);
    
    std::vector<std::uint8_t> buffer;
    rec1.serialize(buffer);
    
    LogRecord rec2;
    std::size_t parsed = rec2.deserialize(buffer.data(), buffer.size());
    
    assert(parsed == buffer.size());
    assert(rec2.verify_checksum());
    assert(rec2.get_txn_id() == 100);
    assert(rec2.get_type() == LogRecordType::Insert);
    assert(rec2.get_prev_lsn().value() == 10);
    assert(rec2.get_payload() == payload);
    
    std::cout << "test_wal_serialization passed.\n";
}

void test_wal_append_read() {
    std::cout << "Running test_wal_append_read...\n";
    std::string test_dir = "test_wal_dir";
    std::filesystem::remove_all(test_dir);
    
    {
        WALManager wal(test_dir, DurabilityMode::Normal);
        
        LogRecord rec1(100, LogRecordType::TxnBegin, LSN::invalid());
        wal.append(rec1);
        
        LogRecord rec2(100, LogRecordType::Insert, LSN::invalid(), LSN::invalid(), {0xAA, 0xBB});
        wal.append(rec2);
        
        wal.flush();
    } // WAL closes and flushes
    
    {
        LogReader reader(test_dir);
        assert(reader.initialize());
        assert(reader.seek_to_first());
        
        auto rec1_opt = reader.read_next();
        assert(rec1_opt.has_value());
        assert(rec1_opt->get_txn_id() == 100);
        assert(rec1_opt->get_type() == LogRecordType::TxnBegin);
        
        auto rec2_opt = reader.read_next();
        assert(rec2_opt.has_value());
        assert(rec2_opt->get_txn_id() == 100);
        assert(rec2_opt->get_type() == LogRecordType::Insert);
        assert(rec2_opt->get_payload().size() == 2);
        assert(rec2_opt->get_payload()[0] == 0xAA);
        
        auto empty_opt = reader.read_next();
        assert(!empty_opt.has_value());
    }
    
    std::filesystem::remove_all(test_dir);
    std::cout << "test_wal_append_read passed.\n";
}

int main() {
    test_wal_serialization();
    test_wal_append_read();
    
    std::cout << "All WAL tests passed!\n";
    return 0;
}
