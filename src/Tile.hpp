#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>

#include <iostream>

enum class TileType
{
	BLANK = -1,
	WATER = 0,
	LAND = 1,
};

/*
TileType WhatTileType(int type)
{
	return static_cast<TileType>(type);
}
*/


class CTile
{
public:
	CTile(int id, int type, sf::Vector2i coords, sf::Vector2f pos, int size);

	~CTile() = default;

	void Init(sf::Vector2f pos);
	void Draw(sf::RenderWindow& window);
	bool MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos);

	sf::RectangleShape GetTile() const;
	sf::Color GetColor() const;
	int GetId() const;

	sf::Vector2i GetCoords() const;

	void SetType(const int type);
	int GetType() const;

	void SetSelected(const bool selected);
	bool GetSelected() const;

	sf::Vector2f GetPosition() const;

private:
	sf::RectangleShape m_tile;
	//sf::Sprite m_tile;

	sf::Color m_color;
	sf::Font m_font;
	sf::Text m_idText;

	sf::Texture m_waterTexture;
	sf::Texture m_landTexture;
	
	sf::Vector2i m_coords;

	int m_id;
	int m_type;
	TileType m_tileType;

	bool m_selected = false;

	void printInfo();

	friend std::ostream& operator<<(std::ostream& os, const CTile& tile);
};

#endif // TILE_HPP
