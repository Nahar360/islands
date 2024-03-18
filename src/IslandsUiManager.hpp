#include <SFML/Graphics.hpp>

#include "IslandsWorld.hpp"

#pragma once

class CIslandsUiManager
{
public:
	CIslandsUiManager() = default;
	~CIslandsUiManager() = default;

	void Init(sf::RenderWindow& window);
	void Shutdown();

	void ProcessEvent(sf::Event event);

	void Run(sf::RenderWindow& window, CIslandsWorld& world, float fps);
	void Render(sf::RenderWindow& window);

private:
	sf::Clock m_deltaClock;
	std::vector<std::string> m_worldsToLoad;

	// UI
	void Update(sf::RenderWindow& window);
	void Begin();
	void HandleUi(sf::RenderWindow& window, CIslandsWorld& world, float fps);
	void End();

	void UpdateWindowTitle(sf::RenderWindow& window);
	void ShowFPS(float fps);
	void UpdateMousePosition(sf::RenderWindow& window);
	void UpdateBackgroundColor();

	void InitialiseWorld(CIslandsWorld& world);
	void InitialiseRandomWorld(CIslandsWorld& world);
	void ClearWorld(CIslandsWorld& world);

	void PrintWorldRepresentation(CIslandsWorld& world);

	void SaveWorld(CIslandsWorld& world);
	void LoadWorld(CIslandsWorld& world);

	void DetectIslands(CIslandsWorld& world);

	// Non-UI
	void GetWorldsToLoad();
};
