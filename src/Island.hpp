#ifndef ISLAND_HPP
#define ISLAND_HPP

#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

class CIsland
{
public:
    CIsland(const std::vector<int>& ids, const std::string& hash);
    ~CIsland() = default;

    std::vector<int> GetIds() const { return m_ids; }
    std::string GetHash() const { return m_hash; }

private:
   std::vector<int> m_ids;
   std::string m_hash;
};

#endif // ISLAND_HPP
