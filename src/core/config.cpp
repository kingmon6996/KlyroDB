#include "klyro/core/config.hpp"
#include "klyro/core/constants.hpp"

namespace klyro {

Config::Config()
    : m_page_size(core::DEFAULT_PAGE_SIZE)
    , m_buffer_pool_size(core::DEFAULT_BUFFER_POOL_SIZE)
    , m_max_connections(100) // Sensible default
{
}

} // namespace klyro
