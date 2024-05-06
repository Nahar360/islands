#include "Island.hpp"

CIsland::CIsland(const std::vector<int>& ids, const std::string& hash) : m_ids(std::move(ids)), m_hash(std::move(hash))
{
}
