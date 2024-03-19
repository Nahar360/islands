#include "World.hpp"

#include "GlobalSettings.hpp"
#include "Tile.hpp"
#include "UiSettings.hpp"

#include <vector>
#include <iostream>
#include <fstream>

void CWorld::Init(int cols, int rows, int size)
{	
	std::vector<std::vector<int>> repr(rows, std::vector<int>(cols, 0));

	InitTilesFromRepr(repr, size);
}

void CWorld::InitRandom(int size)
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

	InitTilesFromRepr(repr, size);
}

void CWorld::Clear()
{
	m_tiles.clear();
}

void CWorld::InitTilesFromRepr(const std::vector<std::vector<int>>& repr, int size)
{
	m_tiles.clear();

	// initialise tiles depending on representation
	for (int i = 0; i < repr.size(); i++)
	{
		std::vector<CTile> tiles_row;
		const int numColumns = repr[i].size();
		for (int j = 0; j < numColumns; j++)
		{
			int tileType = repr[i][j];
			sf::Vector2i coords(i, j);
			sf::Vector2f pos((j + 1) * size, (i + 1) * size);

			CTile tile((i * numColumns) + j, tileType, coords, pos, size);

			tiles_row.push_back(tile);
		}

		m_tiles.push_back(tiles_row);
	}
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
			if (m_tiles[i][j].MouseDetection(mouseButton, mousePos))
			{
				m_tiles[i][j].SetSelected(true);
				int id = m_tiles[i][j].GetId();

				break;
			}
			else
			{
				m_tiles[i][j].SetSelected(false);
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
			std::cout << m_tiles[i][j];
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

			const int id = -1; // will be recalculated later
			const int type = tileType == 2 ? std::rand() % 2 : tileType; // 2 -> random
			const sf::Vector2i coords(lastTileInRow.GetCoords().x, lastTileInRow.GetCoords().y + 1);
			const int size = TILE_SIZE_PIXELS;
			const sf::Vector2f pos(lastTileInRow.GetPosition().x + size, lastTileInRow.GetPosition().y);

			CTile newTile(id, type, coords, pos, size);
			m_tiles[i].emplace_back(newTile);
		}

		// TODO: Ideally this function call would not exist and
		// the ids of the tiles after the ones being added are recalculated in the go
		RecalculateIds();
	}
	else
	{
		// Initialise a world with a single tile
		Init(1, 1);
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

			const int id = -1; // will be recalculated right after
			const int type = tileType == 2 ? std::rand() % 2 : tileType; // 2 -> random
			const sf::Vector2i coords(tileAbove.GetCoords().x + 1, tileAbove.GetCoords().y);
			const int size = TILE_SIZE_PIXELS;
			const sf::Vector2f pos(tileAbove.GetPosition().x, tileAbove.GetPosition().y + size);

			CTile newTile(id, type, coords, pos, size);
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
		Init(1, 1);
	}
}

void CWorld::RemoveRow()
{
	if (!m_tiles.empty())
	{
		m_tiles.pop_back();
	}
}

std::vector<std::vector<CTile>> CWorld::GetTiles() const
{
	return m_tiles;
}

void CWorld::Save(const std::string& worldFileName)
{
	std::string worldFileNamePath = std::string(WORLDS_PATH) + worldFileName;

	std::ofstream worldFile(worldFileNamePath);
	if (worldFile.is_open())
	{
		for (int i = 0; i < m_tiles.size(); i++)
		{
			for (int j = 0; j < m_tiles[0].size(); j++)
			{
				worldFile << std::to_string(m_tiles[i][j].GetType());
			}
			worldFile << std::endl;
		}

		worldFile.close();
	}
}

void CWorld::Load(const std::string& worldFileName)
{
	std::vector<std::vector<int>> repr;

	std::string worldFilePath = std::string(WORLDS_PATH) + worldFileName;
	std::ifstream worldFile(worldFilePath);
	if (worldFile.is_open())
	{
		std::string line;
		while (getline(worldFile, line))
		{
			std::vector<int> row;
			for (int i = 0; i < line.length(); i++)
			{
				int tileType = static_cast<int>(line[i]) - 48; // 48 is the ASCII value of 0
				row.push_back(tileType);
			}
			
			repr.push_back(row);
		}

		worldFile.close();
	}

	InitTilesFromRepr(repr, TILE_SIZE_PIXELS);
}

int CWorld::DetectIslands()
{
	m_islands.clear();

	// TODO: review from scratch
	/*
	for (int i = 0; i < m_tiles.size(); i++)
	{
		for (int j = 0; j < m_tiles[0].size(); j++)
		{
			const int type = m_tiles[i][j].GetType();
			if (type == 1)
			{
				const int tileId = m_tiles[i][j].GetId();
				if (!TileIdAlreadyInIslands(tileId))
				{
					std::vector<int> island{ tileId };
					BuildIslandFromLandTile(m_tiles[i][j], island);
					m_islands.emplace_back(island);
				}
			}
		}
		std::cout << std::endl;
	}

	std::cout << "Number of islands: " << m_islands.size() << std::endl;

	for (int i = 0; i < m_islands.size(); i++)
	{
		std::cout << "Island number " << i + 1 << ": { ";
		for (int j = 0; j < m_islands[i].size(); j++)
		{
			std::cout << m_islands[i][j];
			if (j != m_islands[i].size() - 1)
			{
				std::cout << ", ";
			}
		}
		std::cout << " }" << std::endl;
	}
	*/

	return 0;
}

// Detect Islands helper functions
// TODO: review from scratch along with DetectIslands()
void CWorld::BuildIslandFromLandTile(const CTile& landTile, std::vector<int>& island)
{
	std::vector<int> neighbourTileIds = GetNeighbourTileIds(landTile);
	PrintNeighbourTileIds(neighbourTileIds);

	// ABOVE
	// TODO: can be spared?
	/*if (neighbourTileIds[0] != -1)
	{
		const CTile aboveTile = GetIslandTileFromId(neighbourTileIds[0]);
		if (aboveTile.GetType() == 1)
		{
			if (!TileIdAlreadyInIslands(aboveTile.GetId()))
			{
				island.emplace_back(aboveTile.GetId());
				BuildIslandFromLandTile(aboveTile, island);
			}
		}
	}*/

	// LEFT
	if (neighbourTileIds[1] != -1)
	{
		const CTile leftTile = GetIslandTileFromId(neighbourTileIds[1]);
		if (leftTile.GetType() == 1)
		{
			if (!TileIdAlreadyInIslands(leftTile.GetId()))
			{
				island.emplace_back(leftTile.GetId());
				BuildIslandFromLandTile(leftTile, island);
			}
		}
	}

	// BELOW
	if (neighbourTileIds[2] != -1)
	{
		const CTile belowTile = GetIslandTileFromId(neighbourTileIds[2]);
		if (belowTile.GetType() == 1)
		{
			if (!TileIdAlreadyInIslands(belowTile.GetId()))
			{
				island.emplace_back(belowTile.GetId());
				BuildIslandFromLandTile(belowTile, island);
			}
		}
	}

	// RIGHT
	if (neighbourTileIds[3] != -1)
	{
		const CTile rightTile = GetIslandTileFromId(neighbourTileIds[3]);
		if (rightTile.GetType() == 1)
		{
			if (!TileIdAlreadyInIslands(rightTile.GetId()))
			{
				island.emplace_back(rightTile.GetId());
				BuildIslandFromLandTile(rightTile, island);
			}
		}
	}
}

std::vector<int> CWorld::GetNeighbourTileIds(const CTile& tile)
{
	return {
		GetTileIdWithOffset(tile, {-1, 0}),
		GetTileIdWithOffset(tile, {0, -1}), 
		GetTileIdWithOffset(tile, {1, 0}),
		GetTileIdWithOffset(tile, {0, 1})
	};
}

void CWorld::PrintNeighbourTileIds(const std::vector<int>& neighbourTileIds)
{
	std::cout << "Above: " << neighbourTileIds[0] << std::endl;
	std::cout << "Left: " << neighbourTileIds[1] << std::endl;
	std::cout << "Below: " << neighbourTileIds[2] << std::endl;
	std::cout << "Right: " << neighbourTileIds[3] << std::endl;
}


int CWorld::GetTileIdWithOffset(const CTile& tile, const sf::Vector2i& offset)
{
	const auto& tileCoords = tile.GetCoords();
	const sf::Vector2i otherTileCoord = sf::Vector2i(tileCoords.x + offset.x, tileCoords.y + offset.y);

	if (!IsCoordOutOfBounds(otherTileCoord))
	{
		return m_tiles[otherTileCoord.x][otherTileCoord.y].GetId();
	}

	return -1;
}

bool CWorld::IsCoordOutOfBounds(const sf::Vector2i coord)
{
	if (coord.x >= 0 && coord.y >= 0)
	{
		return false;
	}

	return true;
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

bool CWorld::TileIdAlreadyInIslands(int id)
{
	for (int i = 0; i < m_islands.size(); i++)
	{
		for (int j = 0; j < m_islands[i].size(); j++)
		{
			if (id == m_islands[i][j])
			{
				return true;
			}
		}
	}

	return false;
}
