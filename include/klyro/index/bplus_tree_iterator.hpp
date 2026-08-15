#ifndef KLYRO_INDEX_BPLUS_TREE_ITERATOR_HPP
#define KLYRO_INDEX_BPLUS_TREE_ITERATOR_HPP

#include "klyro/index/index_key.hpp"
#include "klyro/storage/record_id.hpp"
#include "klyro/storage/buffer_pool.hpp"
#include "klyro/index/bplus_tree_leaf.hpp"
#include "klyro/types/type_id.hpp"
#include <memory>
#include <vector>

namespace klyro::index {

class BPlusTreeIterator {
public:
    BPlusTreeIterator() = default;
    
    BPlusTreeIterator(storage::BufferPool* buffer_pool, 
                      PageID leaf_id, 
                      std::uint16_t index, 
                      const std::vector<types::TypeID>& types);

    bool is_valid() const;
    void next();
    
    const IndexKey& key() const;
    storage::RecordID record_id() const;

private:
    storage::BufferPool* m_buffer_pool{nullptr};
    PageID m_current_page_id{};
    std::uint16_t m_current_index{0};
    std::vector<types::TypeID> m_types;
    
    // Lazily fetch the page when we need data
    std::unique_ptr<BPlusTreeLeaf> m_leaf_cache;
    
    mutable IndexKey m_current_key_cache;
    mutable bool m_key_cached{false};

    void load_page();
};

} // namespace klyro::index

#endif // KLYRO_INDEX_BPLUS_TREE_ITERATOR_HPP
