#include "IslandsTile.hpp"

#include <vector>

#pragma once

class CIslandsWorld
{
public:
	CIslandsWorld() = default;
	~CIslandsWorld() = default;

	void Init(int cols, int rows, int size);
	void InitRandom(int size);
	void Clear();
	void Update(sf::RenderWindow& window);
	void MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos);

	void AddColumn();
	void RemoveColumn();
	void AddRow();
	void RemoveRow();

	void PrintRepresentation();

	std::vector<std::vector<CIslandsTile>> GetTiles() const;

	void Save(const std::string& worldFileName);
	void Load(const std::string& worldFileName);

	int DetectIslands();

private:
	int m_cols;
	int m_rows;

	std::vector<std::vector<CIslandsTile>> m_tiles;

	std::vector<std::vector<int>> m_islands;  // vector of vector of tile ids

	void InitTilesFromRepr(const std::vector<std::vector<int>>& repr, int size);

	void BuildIslandFromLandTile(const CIslandsTile& landTile, std::vector<int>& island);

	std::vector<int> GetNeighbourTileIds(const CIslandsTile& tile);
	void PrintNeighbourTileIds(const std::vector<int>& neighbourTileIds);

	int GetTileIdWithOffset(const CIslandsTile& tile, const sf::Vector2i& offset);

	bool IsCoordOutOfBounds(sf::Vector2i coord);

	CIslandsTile GetIslandTileFromId(int id);

	bool TileIdAlreadyInIslands(int id);
};

