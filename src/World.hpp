#ifndef WORLD_HPP
#define WORLD_HPP

#include "Tile.hpp"

#include <vector>

class CWorld
{
public:
	CWorld() = default;
	~CWorld() = default;

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

	std::vector<std::vector<CTile>> GetTiles() const;

	void Save(const std::string& worldFileName);
	void Load(const std::string& worldFileName);

	int DetectIslands();

private:
	std::vector<std::vector<CTile>> m_tiles;

	std::vector<std::vector<int>> m_islands;  // vector of vector of tile ids

	void InitTilesFromRepr(const std::vector<std::vector<int>>& repr, int size);

	void RecalculateIds();

	// Detect Islands helper functions
	void BuildIslandFromLandTile(const CTile& landTile, std::vector<int>& island);
	std::vector<int> GetNeighbourTileIds(const CTile& tile);
	void PrintNeighbourTileIds(const std::vector<int>& neighbourTileIds);
	int GetTileIdWithOffset(const CTile& tile, const sf::Vector2i& offset);
	bool IsCoordOutOfBounds(sf::Vector2i coord);
	CTile GetIslandTileFromId(int id);
	bool TileIdAlreadyInIslands(int id);
};

#endif // WORLD_HPP
