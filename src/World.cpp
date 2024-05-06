#include "World.hpp"

#include <climits> // for INT_MAX
#include <fstream>
#include <iostream>
#include <set>
#include <utility> // for std::pair, std::make_pair
#include <vector>

#include "GlobalSettings.hpp"
#include "Tile.hpp"

void CWorld::Init()
{
    // Load textures
    const std::string texturesPath = std::string(GlobalSettings::TEXTURES_PATH);
    m_waterTexture.loadFromFile(texturesPath + "water.png");
    m_landTexture.loadFromFile(texturesPath + "land.png");
}

void CWorld::InitTiles(int cols, int rows)
{
    std::vector<std::vector<int>> repr(rows, std::vector<int>(cols, 0));

    InitTilesFromRepr(repr);
}

void CWorld::InitTilesRandom()
{
    std::vector<std::vector<int>> repr;

    const int max = 10;
    const int min = 2;
    const int range = (max - min) + 1;

    const int rows = std::rand() % range + min;
    const int cols = std::rand() % range + min;

    for (int i = 0; i < cols; i++)
    {
        std::vector<int> row;
        for (int j = 0; j < rows; j++)
        {
            int tileType = std::rand() % 2;
            row.push_back(tileType);
        }

        repr.push_back(row);
    }

    InitTilesFromRepr(repr);
}

void CWorld::InitTilesFromRepr(const std::vector<std::vector<int>>& repr)
{
    Clear();

    // Initialise tiles depending on representation
    for (int i = 0; i < repr.size(); i++)
    {
        std::vector<CTile> tiles_row;
        const int numColumns = repr[i].size();
        for (int j = 0; j < numColumns; j++)
        {
            const int id = (i * numColumns) + j;
            const int tileType = repr[i][j];
            const sf::Vector2i coords(i, j);
            const int textureSize = m_waterTexture.getSize().x; // y
            const sf::Vector2f pos((j + 1) * textureSize, (i + 1) * textureSize);

            const CTile tile = CreateTile(id, tileType, coords, pos);

            tiles_row.push_back(tile);
        }

        m_tiles.push_back(tiles_row);
    }
}

CTile CWorld::CreateTile(int id, int type, const sf::Vector2i& coords, const sf::Vector2f& pos)
{
    CTile tile(id, static_cast<TileType>(type), coords, pos);
    if (type == 0)
    {
        tile.SetTileTexture(m_waterTexture);
    }
    else if (type == 1)
    {
        tile.SetTileTexture(m_landTexture);
    }

    return tile;
}

void CWorld::Clear()
{
    m_tiles.clear();
    m_islands.clear();
}

void CWorld::RecalculateIds()
{
    for (int i = 0; i < m_tiles.size(); i++)
    {
        const int numColumns = m_tiles[0].size();
        for (int j = 0; j < numColumns; j++)
        {
            const int newId = (i * numColumns) + j;
            m_tiles[i][j].SetId(newId);
        }
    }
}

void CWorld::Update(sf::RenderWindow& window)
{
    for (int i = 0; i < m_tiles.size(); i++)
    {
        for (int j = 0; j < m_tiles[0].size(); j++)
        {
            m_tiles[i][j].Draw(window);
        }
    }
}

void CWorld::MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos)
{
    for (int i = 0; i < m_tiles.size(); i++)
    {
        for (int j = 0; j < m_tiles[0].size(); j++)
        {
            if (m_tiles[i][j].MouseDetection(mouseButton, mousePos, m_waterTexture, m_landTexture))
            {
                break;
            }
        }
    }
}

void CWorld::PrintRepresentation()
{
    for (int i = 0; i < m_tiles.size(); i++)
    {
        for (int j = 0; j < m_tiles[0].size(); j++)
        {
            std::cout << static_cast<int>(m_tiles[i][j].GetType());
        }
        std::cout << std::endl;
    }
}

void CWorld::AddColumn(int tileType)
{
    if (!m_tiles.empty())
    {
        for (int i = 0; i < m_tiles.size(); i++)
        {
            const CTile lastTileInRow = m_tiles[i].back();

            const int id = -1;                                           // will be recalculated later
            const int type = tileType == 2 ? std::rand() % 2 : tileType; // 2 -> random
            const sf::Vector2i coords(lastTileInRow.GetCoords().x, lastTileInRow.GetCoords().y + 1);
            const int textureSize = m_waterTexture.getSize().x; // y
            const sf::Vector2f pos(lastTileInRow.GetPosition().x + textureSize, lastTileInRow.GetPosition().y);

            const CTile newTile = CreateTile(id, type, coords, pos);
            m_tiles[i].emplace_back(newTile);
        }

        // TODO: Ideally this function call would not exist and
        // the ids of the tiles after the ones being added are recalculated in the go
        RecalculateIds();
    }
    else
    {
        // Initialise a world with a single tile
        InitTiles(1, 1);
    }
}

void CWorld::RemoveColumn()
{
    if (!m_tiles.empty())
    {
        for (int i = 0; i < m_tiles.size(); i++)
        {
            m_tiles[i].pop_back();
        }

        // TODO: Ideally this function call would not exist and
        // the ids of the tiles after the ones being removed are recalculated in the go
        RecalculateIds();
    }
}

void CWorld::AddRow(int tileType)
{
    if (!m_tiles.empty())
    {
        std::vector<CTile> newRow;
        std::vector<CTile> lastRow = m_tiles.back();

        for (int i = 0; i < lastRow.size(); i++)
        {
            const CTile tileAbove = lastRow[i];

            const int id = -1;                                           // will be recalculated right after
            const int type = tileType == 2 ? std::rand() % 2 : tileType; // 2 -> random
            const sf::Vector2i coords(tileAbove.GetCoords().x + 1, tileAbove.GetCoords().y);
            const int textureSize = m_waterTexture.getSize().x; // y
            const sf::Vector2f pos(tileAbove.GetPosition().x, tileAbove.GetPosition().y + textureSize);

            const CTile newTile = CreateTile(id, type, coords, pos);
            newRow.emplace_back(newTile);
        }

        m_tiles.emplace_back(newRow);

        // TODO: Ideally this function call would not exist and
        // the ids of the tiles after the ones being added are recalculated in the go
        RecalculateIds();
    }
    else
    {
        // Initialise a world with a single tile
        InitTiles(1, 1);
    }
}

void CWorld::RemoveRow()
{
    if (!m_tiles.empty())
    {
        m_tiles.pop_back();
    }
}

bool CWorld::Save(const std::string& worldFileName)
{
    if (m_tiles.empty())
    {
        return false;
    }

    std::string worldFileNamePath = std::string(GlobalSettings::WORLDS_PATH) + worldFileName;
    std::ofstream worldFile(worldFileNamePath);
    if (worldFile.is_open())
    {
        for (int i = 0; i < m_tiles.size(); i++)
        {
            for (int j = 0; j < m_tiles[0].size(); j++)
            {
                worldFile << std::to_string(static_cast<int>(m_tiles[i][j].GetType()));
            }

            // We do not need a newline at the end of the file
            if (i != m_tiles.size() - 1)
            {
                worldFile << std::endl;
            }
        }

        worldFile.close();
    }

    return true;
}

void CWorld::Load(const std::string& worldFileName)
{
    std::vector<std::vector<int>> repr;

    std::string worldFilePath = std::string(GlobalSettings::WORLDS_PATH) + worldFileName;
    std::ifstream worldFile(worldFilePath);
    if (worldFile.is_open())
    {
        std::string line;
        while (getline(worldFile, line))
        {
            std::vector<int> row;
            for (int i = 0; i < line.length(); i++)
            {
                const int tileType = static_cast<int>(line[i]) - 48; // 48 is the ASCII value of 0
                row.push_back(tileType);
            }

            repr.push_back(row);
        }

        worldFile.close();
    }

    InitTilesFromRepr(repr);
}

std::vector<std::pair<std::vector<int>, std::string>> CWorld::DetectIslands()
{
    m_islands.clear();

    for (int i = 0; i < m_tiles.size(); i++)
    {
        for (int j = 0; j < m_tiles[0].size(); j++)
        {
            if (m_tiles[i][j].IsTypeLand())
            {
                const int tileId = m_tiles[i][j].GetId();
                if (!TileIdAlreadyInIslands(tileId))
                {
                    std::vector<int> island{tileId};
                    std::string hash("S"); // initialise it with "S" (Start) so hashes are not empty for 1-tile islands
                    ExploreIslandFromLandTile(m_tiles[i][j], island, hash);
                    hash += "E"; // wrap it up with "E" (End) for completeness

                    // We could use hashes to compute the number of unique islands by inserting them in a set
                    // This would save the whole GetNumUniqueIslands() stuff, but keeping it as it is interesting

                    m_islands.emplace_back(std::make_pair(island, hash));
                }
            }
        }
    }

    return m_islands;
}

void CWorld::ExploreIslandFromLandTile(const CTile& landTile, std::vector<int>& island, std::string& hash)
{
    std::cout << "Exploring island from land tile with ID: " << landTile.GetId() << std::endl;

    std::vector<int> neighbourTileIds = GetNeighbourTileIds(landTile);
    PrintNeighbourTileIds(neighbourTileIds);

    for (int i = 0; i < neighbourTileIds.size(); i++)
    {
        if (neighbourTileIds[i] != -1)
        {
            const CTile tile = GetIslandTileFromId(neighbourTileIds[i]);
            if (tile.IsTypeLand())
            {
                if (!TileIdAlreadyInAGivenIsland(tile.GetId(), island))
                {
                    // Append id to island
                    island.emplace_back(tile.GetId());

                    // Append direction to hash
                    // clang-format off
                    if      (i == 0) hash += "U";
                    else if (i == 1) hash += "L";
                    else if (i == 2) hash += "D";
                    else if (i == 3) hash += "R";
                    // clang-format on

                    // Keep exploring the island
                    ExploreIslandFromLandTile(tile, island, hash);
                }
            }
        }
    }
}

std::vector<int> CWorld::GetNeighbourTileIds(const CTile& tile)
{
    // clang-format off
    const int aboveTileId = GetTileIdWithOffset(tile, {-1, 0});
    const int leftTileId  = GetTileIdWithOffset(tile, {0, -1});
    const int belowTileId = GetTileIdWithOffset(tile, {1, 0});
    const int rightTileId = GetTileIdWithOffset(tile, {0, 1});
    // clang-format on

    return {aboveTileId, leftTileId, belowTileId, rightTileId};
}

void CWorld::PrintNeighbourTileIds(const std::vector<int>& neighbourTileIds)
{
    // clang-format off
    std::cout << "Above: " << neighbourTileIds[0] << std::endl;
    std::cout << "Left: "  << neighbourTileIds[1] << std::endl;
    std::cout << "Below: " << neighbourTileIds[2] << std::endl;
    std::cout << "Right: " << neighbourTileIds[3] << std::endl;
    // clang-format on
    std::cout << std::endl;
}

int CWorld::GetTileIdWithOffset(const CTile& tile, const sf::Vector2i& offset)
{
    const auto& tileCoords = tile.GetCoords();
    const sf::Vector2i otherTileCoord = sf::Vector2i(tileCoords.x + offset.x, tileCoords.y + offset.y);

    if (IsCoordInBounds(otherTileCoord))
    {
        const int otherTileId = m_tiles[otherTileCoord.x][otherTileCoord.y].GetId();
        return otherTileId;
    }

    return -1;
}

bool CWorld::IsCoordInBounds(const sf::Vector2i coord)
{
    const int numCols = m_tiles[coord.x].size();
    const int numRows = m_tiles.size();

    const bool xCoordIsNotNegative = coord.x >= 0;
    const bool yCoordIsNotNegative = coord.y >= 0;
    const bool xCoordIsNotBiggerThanNumCols = coord.x < numRows;
    const bool yCoordIsNotBiggerThanNumRows = coord.y < numCols;

    const bool isCoordInBounds =
        xCoordIsNotNegative && yCoordIsNotNegative && xCoordIsNotBiggerThanNumCols && yCoordIsNotBiggerThanNumRows;

    return isCoordInBounds;
}

CTile CWorld::GetIslandTileFromId(int id)
{
    for (int i = 0; i < m_tiles.size(); i++)
    {
        for (int j = 0; j < m_tiles[0].size(); j++)
        {
            if (id == m_tiles[i][j].GetId())
            {
                return m_tiles[i][j];
            }
        }
    }

    // TODO
    // add return statement
    // currently: not all control paths return a value (warning)
}

int CWorld::GetTileIdFromTileCoords(const sf::Vector2i& coords)
{
    int tileId = -1;

    for (int i = 0; i < m_tiles.size(); i++)
    {
        for (int j = 0; j < m_tiles[0].size(); j++)
        {
            if (coords == m_tiles[i][j].GetCoords())
            {
                tileId = m_tiles[i][j].GetId();
                break;
            }
        }
    }

    return tileId;
}

bool CWorld::TileIdAlreadyInIslands(int id)
{
    for (int i = 0; i < m_islands.size(); i++)
    {
        for (int j = 0; j < m_islands[i].first.size(); j++)
        {
            if (id == m_islands[i].first[j])
            {
                return true;
            }
        }
    }

    return false;
}

bool CWorld::TileIdAlreadyInAGivenIsland(int id, const std::vector<int>& island)
{
    for (int i = 0; i < island.size(); i++)
    {
        if (id == island[i])
        {
            return true;
        }
    }

    return false;
}

int CWorld::GetNumUniqueIslands()
{
    std::vector<std::vector<int>> islandsShifted; // vector of vector of tile ids

    // First, temporarily create a vector of islands coordinates shifted to the origin
    for (int i = 0; i < m_islands.size(); i++)
    {
        const std::vector<int> islandIds = m_islands[i].first;

        int minX = INT_MAX;
        int minY = INT_MAX;
        for (int j = 0; j < islandIds.size(); j++)
        {
            // Get the coords of each tile
            const CTile islandTile = GetIslandTileFromId(islandIds[j]);
            const sf::Vector2i islandTileCoords = islandTile.GetCoords();

            // Get the min X and min Y coordinates
            const int xCoord = islandTileCoords.x;
            const int yCoord = islandTileCoords.y;

            if (xCoord < minX)
            {
                minX = xCoord;
            }

            if (yCoord < minY)
            {
                minY = yCoord;
            }
        }

        std::vector<int> islandShifted;
        // Shift the whole island relative to the origin
        for (int j = 0; j < islandIds.size(); j++)
        {
            const CTile islandTile = GetIslandTileFromId(islandIds[j]);
            // Now, subtract those minimums from each coordinate
            const sf::Vector2i shiftedCoords = islandTile.GetCoords() - sf::Vector2i{minX, minY};
            const int islandTileId = GetTileIdFromTileCoords(shiftedCoords);
            // If, for whatever reason, it was not possible to get the id from the coords, it does not get added to the shifted vector
            if (islandTileId != -1)
            {
                islandShifted.push_back(islandTileId);
            }
        }

        islandsShifted.emplace_back(islandShifted);
    }

    // Now we have all islands (ids) shifted to the origin
    // we insert them to a set to get unique ones
    std::set<std::vector<int>> uniqueIslands;
    for (const std::vector<int>& islandShifted : islandsShifted)
    {
        uniqueIslands.insert(islandShifted);
    }

    return uniqueIslands.size();
}
