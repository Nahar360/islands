#ifndef WORLD_HPP
#define WORLD_HPP

#include <vector>

#include "Tile.hpp"

class CWorld
{
public:
    CWorld() = default;
    ~CWorld() = default;

    void Init();

    void InitTiles(int cols, int rows);
    void InitTilesRandom();
    void Clear();
    void Update(sf::RenderWindow& window);
    void MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos);

    void AddColumn(int tileType);
    void RemoveColumn();
    void AddRow(int tileType);
    void RemoveRow();

    void PrintRepresentation();

    void Save(const std::string& worldFileName);
    void Load(const std::string& worldFileName);

    std::vector<std::vector<int>> DetectIslands();

    int GetNumUniqueIslands();

private:
    // Textures
    sf::Texture m_waterTexture;
    sf::Texture m_landTexture;

    // Tiles
    std::vector<std::vector<CTile>> m_tiles;

    // Islands
    std::vector<std::vector<int>> m_islands; // vector of vector of tile ids

    void InitTilesFromRepr(const std::vector<std::vector<int>>& repr);
    CTile CreateTile(int id, int type, const sf::Vector2i& coords, const sf::Vector2f& pos);

    void RecalculateIds();

    // Detect Islands helper functions
    void ExploreIslandFromLandTile(const CTile& landTile, std::vector<int>& island);
    std::vector<int> GetNeighbourTileIds(const CTile& tile);
    void PrintNeighbourTileIds(const std::vector<int>& neighbourTileIds);
    int GetTileIdWithOffset(const CTile& tile, const sf::Vector2i& offset);
    bool IsCoordInBounds(sf::Vector2i coord);
    CTile GetIslandTileFromId(int id);

    bool TileIdAlreadyInIslands(int id);
    bool TileIdAlreadyInAGivenIsland(int id, const std::vector<int>& island);
};

#endif // WORLD_HPP
