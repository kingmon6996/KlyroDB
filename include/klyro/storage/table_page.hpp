#ifndef KLYRO_STORAGE_TABLE_PAGE_HPP
#define KLYRO_STORAGE_TABLE_PAGE_HPP

#include "klyro/storage/page_handle.hpp"
#include "klyro/wal/lsn.hpp"
#include "klyro/core/ids.hpp"
#include "klyro/core/result.hpp"
#include "klyro/storage/record.hpp"
#include "klyro/storage/record_view.hpp"
#include "klyro/storage/record_id.hpp"
#include "klyro/storage/tuple_layout.hpp"
#include "klyro/core/status.hpp"
#include <optional>

namespace klyro::storage {

// The header specific to a table page. Sits directly after the generic PageHeader.
struct TablePageHeader {
    PageID next_page_id{};
    PageID prev_page_id{};
    std::uint32_t slot_count{0};
    std::uint32_t free_space_lower_bound{0}; // Points to end of slot array
    std::uint32_t free_space_upper_bound{0}; // Points to beginning of records
    std::uint32_t live_record_count{0};
};

class TablePage {
public:
    // Wraps an existing page handle.
    explicit TablePage(PageHandle handle);

    // Initialize a new empty table page
    void init();

    PageID page_id() const { return m_handle.get().id(); }
    
    // Calculates total usable free space (accounting for fragmentation/slots)
    std::size_t free_space() const;

    // Record operations
    Result<RecordID> insert(const std::vector<std::byte>& record_bytes);
    Result<std::span<const std::byte>> get(const RecordID& id) const;
    Result<void> update(const RecordID& id, const std::vector<std::byte>& record_bytes);
    Result<void> erase(const RecordID& id);

    Result<void> compact();
    
    // WAL Support
    void set_lsn(wal::LSN lsn);
    wal::LSN lsn() const;
    
    // Page linking
    PageID next_page_id() const;
    void set_next_page_id(PageID id);
    PageID prev_page_id() const;
    void set_prev_page_id(PageID id);

private:
    PageHandle m_handle;

    TablePageHeader* header();
    const TablePageHeader* header() const;
    
    std::size_t page_size() const;
};

} // namespace klyro::storage

#endif // KLYRO_STORAGE_TABLE_PAGE_HPP
