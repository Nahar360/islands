#ifndef UI_MANAGER_HPP
#define UI_MANAGER_HPP

#include <SFML/Graphics.hpp>

#include "World.hpp"

class CUiManager
{
public:
    CUiManager() = default;
    ~CUiManager() = default;

    void Init(sf::RenderWindow& window);
    void Shutdown();

    void ProcessEvent(sf::Event event);

    void Run(sf::RenderWindow& window, CWorld& world, float fps);
    void Render(sf::RenderWindow& window);

private:
    sf::Clock m_deltaClock;

    std::vector<std::string> m_worldsToLoad;
    std::vector<std::string> m_tileTypesAvailable{"water", "land", "random"};

    // UI
    void Update(sf::RenderWindow& window);
    void Begin();
    void HandleUi(sf::RenderWindow& window, CWorld& world, float fps);
    void End();

    void UpdateWindowTitle(sf::RenderWindow& window);
    void ShowFPS(float fps);
    void UpdateMousePosition(sf::RenderWindow& window);
    void UpdateBackgroundColor();

    void InitialiseWorld(CWorld& world);
    void InitialiseRandomWorld(CWorld& world);
    void EditWorld(CWorld& world);
    void ClearWorld(CWorld& world);

    void PrintWorldRepresentation(CWorld& world);

    void SaveWorld(CWorld& world);
    void LoadWorld(CWorld& world);

    void DetectIslands(CWorld& world);
    void PrintIslandsSummary();

    // Non-UI
    void GetWorldsToLoad();
};

#endif // UI_MANAGER_HPP
