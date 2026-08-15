#ifndef KLYRO_INDEX_BPLUS_TREE_LEAF_HPP
#define KLYRO_INDEX_BPLUS_TREE_LEAF_HPP

#include "klyro/index/bplus_tree_node.hpp"
#include "klyro/index/index_entry.hpp"
#include "klyro/types/type_id.hpp"

namespace klyro::index {

class BPlusTreeLeaf : public BPlusTreeNode {
public:
    explicit BPlusTreeLeaf(storage::PageHandle handle);

    void init();

    PageID next_page_id() const { return header()->next_page_id; }
    void set_next_page_id(PageID id) { header()->next_page_id = id; m_handle.mark_dirty(); }
    
    PageID prev_page_id() const { return header()->prev_page_id; }
    void set_prev_page_id(PageID id) { header()->prev_page_id = id; m_handle.mark_dirty(); }

    // Returns the exact entry if found, or the index where it should be inserted
    // bool true = exact match found
    std::pair<std::uint16_t, bool> find_lower_bound(const IndexKey& key) const;

    // Inserts maintaining sorted order. Returns false if not enough space.
    bool insert(const IndexKey& key, storage::RecordID record_id);
    
    // Removes exact key + record_id
    bool remove(const IndexKey& key, storage::RecordID record_id);

    LeafEntry entry_at(std::uint16_t index, const std::vector<types::TypeID>& types) const;
    
    // Returns key bytes directly, useful for internal copying/splitting without deserializing
    std::span<const std::byte> key_bytes_at(std::uint16_t index) const;
    storage::RecordID record_id_at(std::uint16_t index) const;

    // Moves half of the entries to the given new leaf. Returns the separator key.
    IndexKey split(BPlusTreeLeaf& new_leaf, const std::vector<types::TypeID>& types);
};

} // namespace klyro::index

#endif // KLYRO_INDEX_BPLUS_TREE_LEAF_HPP
