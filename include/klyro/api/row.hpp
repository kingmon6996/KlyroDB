#ifndef KLYRO_API_ROW_HPP
#define KLYRO_API_ROW_HPP

#include "klyro/types/value.hpp"
#include <memory>
#include <string_view>
#include <vector>

namespace klyro::api {

class RowImpl;

class Row {
public:
    Row(Row&&) noexcept;
    Row& operator=(Row&&) noexcept;
    ~Row();

    // Positional access
    types::Value get(std::size_t index) const;
    types::Value operator[](std::size_t index) const { return get(index); }
    
    // Named access
    types::Value get(std::string_view name) const;
    
    std::size_t size() const noexcept;

private:
    friend class ResultImpl;
    explicit Row(std::unique_ptr<RowImpl> impl);
    
    std::unique_ptr<RowImpl> m_impl;
};

} // namespace klyro::api

#endif // KLYRO_API_ROW_HPP
