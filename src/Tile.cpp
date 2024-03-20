#include "Tile.hpp"

#include <iostream>

CTile::CTile(int id , int type, sf::Vector2i coords, sf::Vector2f pos) :
	m_id(id), m_type(type), m_coords(coords)
{
	// Load font
	m_font.loadFromFile("./src/fonts/arial.ttf");

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
			if (m_type == 0)
			{
				printInfo();

				m_type = 1;
				SetTileTexture(landTexture);
			}
			return true;
		}
		case sf::Mouse::Button::Right:
		{
			if (m_type == 1)
			{
				printInfo();

				m_type = 0;
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

sf::Color CTile::GetColor() const
{
	return m_color;
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

sf::Vector2i CTile::GetCoords() const
{
	return m_coords;
}

int CTile::GetType() const
{
	return m_type;
}

void CTile::SetType(const int type)
{
	m_type = type;
}

bool CTile::GetSelected() const
{
	return m_selected;
}

void CTile::SetSelected(const bool selected)
{
	m_selected = selected;
}

sf::Vector2f CTile::GetPosition() const
{
	return m_tile.getPosition();
}

void CTile::printInfo()
{
	std::cout << "Tile" << std::endl;
	std::cout << "Id: " << m_id << std::endl;
	std::cout << "Type: " << m_type << std::endl;
	std::cout << "Coords.: [" << m_coords.x << ", " << m_coords.y << "]" << std::endl;
	std::cout << std::endl;
}

// -------

std::ostream& operator<<(std::ostream& os, const CTile& tile)
{
	os << tile.GetType();
	return os;
}
