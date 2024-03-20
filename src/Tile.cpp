#include "Tile.hpp"

#include "GlobalSettings.hpp"

#include "SFML/System/Vector2.hpp"

#include <iostream>

CTile::CTile(int id , TileType type, sf::Vector2i coords, sf::Vector2f pos) :
	m_id(id), m_type(type), m_coords(coords)
{
	// Load font
	m_font.loadFromFile(std::string(FONTS_PATH) + "arial.ttf");

	// Position
	m_tile.setPosition(pos);

	// Text
	m_idText.setString(std::to_string(GetId()));
	m_idText.setCharacterSize(16);
	m_idText.setFillColor(sf::Color::Black);
	m_idText.setStyle(sf::Text::Bold);
	m_idText.setPosition(m_tile.getPosition().x, m_tile.getPosition().y);
}

void CTile::SetTileTexture(const sf::Texture& texture)
{
	m_tile.setTexture(texture);
}

void CTile::Draw(sf::RenderWindow& window)
{
	// Tile
	window.draw(m_tile);

	// Text
	m_idText.setFont(m_font);
	window.draw(m_idText);
}

// TODO: I do not like having to pass all textures here..., find a way around this
// The problem is that the textures are created and stored in the world and not in the tiles...
bool CTile::MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos, const sf::Texture& waterTexture, const sf::Texture& landTexture)
{
	sf::Vector2f mousePosFloat = sf::Vector2f(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
	if (m_tile.getGlobalBounds().contains(mousePosFloat))
	{
		switch (mouseButton)
		{
		case sf::Mouse::Button::Left:
		{
			if (m_type == TileType::WATER)
			{
				printInfo();

				m_type = TileType::LAND;
				SetTileTexture(landTexture);
			}
			return true;
		}
		case sf::Mouse::Button::Right:
		{
			if (m_type == TileType::LAND)
			{
				printInfo();

				m_type = TileType::WATER;
				SetTileTexture(waterTexture);
			}
			return true;
		}
		default:
			break;
		}
	}

	return false;
}

void CTile::SetId(int id)
{
	m_id = id;
	m_idText.setString(std::to_string(id));
}

int CTile::GetId() const
{
	return m_id;
}

TileType CTile::GetType() const
{
	return m_type;
}

void CTile::SetType(TileType type)
{
	m_type = type;
}

bool CTile::IsTypeLand() const
{
	return m_type == TileType::LAND;
}

void CTile::SetCoords(const sf::Vector2i &coords)
{
	m_coords = coords;
}

sf::Vector2i CTile::GetCoords() const
{
	return m_coords;
}

void CTile::SetPosition(const sf::Vector2f &pos)
{
	m_tile.setPosition(pos);
}

sf::Vector2f CTile::GetPosition() const
{
	return m_tile.getPosition();
}

bool CTile::GetSelected() const
{
	return m_selected;
}

void CTile::SetSelected(bool selected)
{
	m_selected = selected;
}

sf::Vector2f CTile::GetSize() const
{
	return m_tile.getScale();
}

void CTile::printInfo()
{
	std::cout << "Tile" << std::endl;
	std::cout << "Id: " << m_id << std::endl;
	std::cout << "Type: " << static_cast<int>(m_type) << std::endl;
	std::cout << "Coords.: [" << m_coords.x << ", " << m_coords.y << "]" << std::endl;
	std::cout << std::endl;
}

// -------

std::ostream& operator<<(std::ostream& os, const CTile& tile)
{
	os << static_cast<int>(tile.GetType());
	return os;
}
