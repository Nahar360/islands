#include "IslandsGame.hpp"

#include "GlobalSettings.hpp"

CIslandsGame::CIslandsGame() :
	m_window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), ISLANDS_WINDOW_TITLE)
{
	srand(time(NULL));
}

void CIslandsGame::Run()
{
	m_uiManager.Init(m_window);

	sf::Clock clock = sf::Clock();
	sf::Time previousTime = clock.getElapsedTime();
	sf::Time currentTime;

	while (m_window.isOpen())
	{
		currentTime = clock.getElapsedTime();
		float fps = 1.0f / (currentTime.asSeconds() - previousTime.asSeconds());
		previousTime = currentTime;

		CheckEvents();

		m_uiManager.Run(m_window, m_world, fps);

		m_window.clear(BACKGROUND_COLOR);

		m_world.Update(m_window);

		m_uiManager.Render(m_window);

		m_window.display();
	}

	m_uiManager.Shutdown();
}

void CIslandsGame::CheckEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		m_uiManager.ProcessEvent(event);

		if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			m_window.close();
		}
	}

	if (event.type == sf::Event::MouseButtonPressed)
	{
		m_world.MouseDetection(event.mouseButton.button, sf::Mouse::getPosition(m_window));
	}
}
