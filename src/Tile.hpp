#ifndef TILE_HPP
#define TILE_HPP

#include "TileType.hpp"

#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>

#include <iostream>

class CTile
{
public:
	CTile(int id, TileType type, sf::Vector2i coords, sf::Vector2f pos);

	~CTile() = default;

	void SetTileTexture(const sf::Texture& texture);
	void Draw(sf::RenderWindow& window);
	bool MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos, const sf::Texture& waterTexture, const sf::Texture& landTexture);

	void SetId(int id);
	int GetId() const;

	void SetType(TileType type);
	TileType GetType() const;

	bool IsTypeWater() const;
	bool IsTypeLand() const;

	void SetCoords(const sf::Vector2i& coords);
	sf::Vector2i GetCoords() const;

	void SetPosition(const sf::Vector2f& pos);
	sf::Vector2f GetPosition() const;

	sf::Vector2f GetSize() const;

private:
	sf::Sprite m_tile;

	sf::Font m_font;
	sf::Text m_idText;
	
	int m_id;
	TileType m_type;
	sf::Vector2i m_coords;

	// -------

	friend std::ostream& operator<<(std::ostream& os, const CTile& tile);
};

#endif // TILE_HPP
