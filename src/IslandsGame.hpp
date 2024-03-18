#include <SFML/Graphics.hpp>

#include "IslandsUiManager.hpp"
#include "IslandsWorld.hpp"

#pragma once

class CIslandsGame
{
public:
	CIslandsGame();
	~CIslandsGame() = default;

	void Run();

private:
	sf::RenderWindow m_window;

	CIslandsUiManager m_uiManager;
	CIslandsWorld m_world;

	bool m_mouseHasBeenPressed = false;

	void CheckEvents();
};

