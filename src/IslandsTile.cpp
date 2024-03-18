#include "IslandsTile.hpp"

#include "GlobalSettings.hpp"
#include "UiSettings.hpp"

#include <iostream>

CIslandsTile::CIslandsTile(int id , int type, sf::Vector2i coords, sf::Vector2f pos, int size) :
	m_id(id), m_type(type), m_coords(coords)
{
	// Load textures
	m_waterTexture.loadFromFile("./src/water.png"); // TODO: move to /textures
	m_landTexture.loadFromFile("./src/land.png"); // TODO: move to /textures

	// Load font
	m_font.loadFromFile("./src/arial.ttf"); // TODO: move to /fonts

	// -----------

	m_tile.setSize(sf::Vector2f(size, size));

	m_tile.setOutlineColor(sf::Color::Black);

	// Water
	if (m_type == 0)
	{
		m_tile.setFillColor(sf::Color::Blue);
		//m_tile.setTexture(m_waterTexture);
	}
	// Land
	else if (m_type == 1)
	{
		m_tile.setFillColor(sf::Color(165, 42, 42));
		//m_tile.setTexture(m_landTexture);
	}

	m_tile.setOutlineThickness(1);
	m_tile.setPosition(pos);

	// Text
	m_idText.setString(std::to_string(GetId()));
	m_idText.setCharacterSize(12);
	m_idText.setFillColor(sf::Color::Black);
	// m_idText.setStyle(sf::Text::Bold);
	m_idText.setPosition(m_tile.getPosition().x, m_tile.getPosition().y);
}

void CIslandsTile::Draw(sf::RenderWindow& window)
{
	// Tile
	window.draw(m_tile);

	// Text
	m_idText.setFont(m_font);
	window.draw(m_idText);
}


bool CIslandsTile::MouseDetection(sf::Mouse::Button mouseButton, sf::Vector2i mousePos)
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
				m_tile.setFillColor(sf::Color(165, 42, 42));
			}
			return true;
		}
		case sf::Mouse::Button::Right:
		{
			if (m_type == 1)
			{
				printInfo();

				m_type = 0;
				m_tile.setFillColor(sf::Color::Blue);
			}
			return true;
		}
		default:
			break;
		}
	}

	return false;
}

sf::RectangleShape CIslandsTile::GetTile() const
{
	return m_tile;
}

sf::Color CIslandsTile::GetColor() const
{
	return m_color;
}

int CIslandsTile::GetId() const
{
	return m_id;
}

sf::Vector2i CIslandsTile::GetCoords() const
{
	return m_coords;
}

int CIslandsTile::GetType() const
{
	return m_type;
}

void CIslandsTile::SetType(const int type)
{
	m_type = type;
}

bool CIslandsTile::GetSelected() const
{
	return m_selected;
}

void CIslandsTile::SetSelected(const bool selected)
{
	m_selected = selected;
}

sf::Vector2f CIslandsTile::GetPosition() const
{
	return m_tile.getPosition();
}

void CIslandsTile::printInfo()
{
	std::cout << "Tile" << std::endl;
	std::cout << "Id: " << m_id << std::endl;
	std::cout << "Type: " << m_type << std::endl;
	std::cout << "Coords.: [" << m_coords.x << ", " << m_coords.y << "]" << std::endl;
	std::cout << std::endl;
}

// -------

std::ostream& operator<<(std::ostream& os, const CIslandsTile& tile)
{
	os << tile.GetType();
	return os;
}