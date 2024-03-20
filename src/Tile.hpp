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
	CTile(int id, int type, sf::Vector2i coords, sf::Vector2f pos);

	~CTile() = default;

	void SetTileTexture(const sf::Texture& texture);

	void Draw(sf::RenderWindow& window);
	bool MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos, const sf::Texture& waterTexture, const sf::Texture& landTexture);

	sf::Color GetColor() const;

	void SetId(int id);
	int GetId() const;

	sf::Vector2i GetCoords() const;

	void SetType(const int type);
	int GetType() const;

	void SetSelected(const bool selected);
	bool GetSelected() const;

	sf::Vector2f GetSize() const { return m_tile.getScale(); }

	sf::Vector2f GetPosition() const;

private:
	sf::Sprite m_tile;

	sf::Color m_color;
	sf::Font m_font;
	sf::Text m_idText;
	
	int m_id;
	int m_type;
	TileType m_tileType;
	sf::Vector2i m_coords;	

	bool m_selected = false;

	void printInfo();

	friend std::ostream& operator<<(std::ostream& os, const CTile& tile);
};

#endif // TILE_HPP
