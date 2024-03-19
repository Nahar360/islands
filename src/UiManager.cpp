#include <filesystem>

#include "UiManager.hpp"

#include "GlobalSettings.hpp"
#include "UiSettings.hpp"

#include "imgui.h"
#include "imgui-SFML.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

void CUiManager::Init(sf::RenderWindow& window)
{
	ImGui::SFML::Init(window);

	GetWorldsToLoad();
}

void CUiManager::Shutdown()
{
	ImGui::SFML::Shutdown();
}

void CUiManager::ProcessEvent(sf::Event event)
{
	ImGui::SFML::ProcessEvent(event);
}

void CUiManager::Update(sf::RenderWindow& window)
{
	ImGui::SFML::Update(window, m_deltaClock.restart());
}

void CUiManager::Begin()
{
	ImGui::Begin("Menu");
}

void CUiManager::Run(sf::RenderWindow& window, CWorld& world, float fps)
{
	Update(window);

	Begin();

	HandleUi(window, world, fps);

	End();
}

void CUiManager::HandleUi(sf::RenderWindow& window, CWorld& world, float fps)
{
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "General settings");

	UpdateWindowTitle(window);

	ShowFPS(fps);

	UpdateMousePosition(window);

	// -------------------------
	ImGui::Separator();
	// -------------------------

	ImGui::TextColored(ImVec4(1, 1, 0, 1), "World settings");

	ImGui::TextColored(ImVec4(1, 1, 0.5, 1), "Create World");

	ImGui::InputInt("Tile size (pixels)", &ISLANDS_TILE_SIZE_PIXELS);

	InitialiseWorld(world);
	ImGui::SameLine();
	InitialiseRandomWorld(world);
	
	ImGui::TextColored(ImVec4(1, 1, 0.5, 1), "Edit World");
	EditWorld(world);

	ClearWorld(world);
	ImGui::SameLine();
	PrintWorldRepresentation(world);

	SaveWorld(world);
	LoadWorld(world);

	// -------------------------
	ImGui::Separator();
	// -------------------------

	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Islands");

	DetectIslands(world);
}

void CUiManager::End()
{
	ImGui::End();
}

void CUiManager::Render(sf::RenderWindow& window)
{
	ImGui::SFML::Render(window);
}

void CUiManager::UpdateWindowTitle(sf::RenderWindow& window)
{
	if (ImGui::InputText("Window title", ISLANDS_WINDOW_TITLE, 255))
	{
		window.setTitle(ISLANDS_WINDOW_TITLE);
	}
}

void CUiManager::ShowFPS(float fps)
{
	ImGui::Text("FPS: %f", fps);
}

void CUiManager::UpdateMousePosition(sf::RenderWindow& window)
{
	sf::Vector2i mousePos = sf::Mouse::getPosition(window);
	if (mousePos.x >= 0 && mousePos.x <= WINDOW_WIDTH &&
		mousePos.y >= 0 && mousePos.y <= WINDOW_HEIGHT)
	{
		ImGui::Text("Mouse position: (%d, %d)", mousePos.x, mousePos.y);
	}
	else
	{
		ImGui::Text("Mouse out of window");
	}
}

void CUiManager::UpdateBackgroundColor()
{
	if (ImGui::ColorEdit3("Background color", BACKGROUND_COLOR_INPUT))
	{
		BACKGROUND_COLOR.r = static_cast<sf::Uint8>(BACKGROUND_COLOR_INPUT[0] * 255.f);
		BACKGROUND_COLOR.g = static_cast<sf::Uint8>(BACKGROUND_COLOR_INPUT[1] * 255.f);
		BACKGROUND_COLOR.b = static_cast<sf::Uint8>(BACKGROUND_COLOR_INPUT[2] * 255.f);
	}
}

void CUiManager::InitialiseWorld(CWorld& world)
{
	ImGui::InputInt("No. cols", &ISLANDS_WORLD_COLS);
	ImGui::InputInt("No. rows", &ISLANDS_WORLD_ROWS);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.4f, 0.1f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.6f, 0.1f, 1.0f));
	if (ImGui::Button("Init. world"))
	{
		world.Init(ISLANDS_WORLD_COLS, ISLANDS_WORLD_ROWS, ISLANDS_TILE_SIZE_PIXELS);
	}
	ImGui::PopStyleColor(2);
}

void CUiManager::InitialiseRandomWorld(CWorld& world)
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.7f, 0.4f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));
	if (ImGui::Button("Init. random world"))
	{
		world.InitRandom(ISLANDS_TILE_SIZE_PIXELS);
	}
	ImGui::PopStyleColor(2);
}

void CUiManager::EditWorld(CWorld& world)
{
	if (ImGui::BeginCombo("Add (types)", m_tileTypesAvailable[ISLANDS_WORLD_CURRENT_TYPE].data(), 0))
	{
		for (int n = 0; n < m_tileTypesAvailable.size(); n++)
		{
			const bool is_selected = (ISLANDS_WORLD_CURRENT_TYPE == n);
			if (ImGui::Selectable(m_tileTypesAvailable[n].data(), is_selected))
			{
				ISLANDS_WORLD_CURRENT_TYPE = n;
			}

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1), "Cols");
	ImGui::SameLine();
	if (ImGui::ArrowButton("##leftCol", ImGuiDir_Left)) { world.RemoveColumn(); }
	ImGui::SameLine();
	if (ImGui::ArrowButton("##rightCol", ImGuiDir_Right)) { world.AddColumn(ISLANDS_WORLD_CURRENT_TYPE); }

	ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1), "Rows");
	ImGui::SameLine();
	if (ImGui::ArrowButton("##leftRow", ImGuiDir_Left)) { world.RemoveRow(); }
	ImGui::SameLine();
	if (ImGui::ArrowButton("##rightRow", ImGuiDir_Right)) { world.AddRow(ISLANDS_WORLD_CURRENT_TYPE); }
}

void CUiManager::ClearWorld(CWorld& world)
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.9f, 0.2f, 0.1f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.2f, 0.1f, 1.0f));
	if (ImGui::Button("Clear world"))
	{
		world.Clear();
	}
	ImGui::PopStyleColor(2);
}

void CUiManager::PrintWorldRepresentation(CWorld& world)
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.1f, 0.6f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.3f, 0.6f, 1.0f));
	if (ImGui::Button("Print world"))
	{
		world.PrintRepresentation();
	}
	ImGui::PopStyleColor(2);
}

void CUiManager::SaveWorld(CWorld& world)
{
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Save");

	ImGui::InputText("World to save", ISLANDS_WORLD_FILE_NAME_TO_SAVE, 255);

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.5f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.7f, 0.2f, 1.0f));
	if (ImGui::Button("Save world (to .txt file)"))
	{
		world.Save(std::string(ISLANDS_WORLD_FILE_NAME_TO_SAVE));
		GetWorldsToLoad();
	}
	ImGui::PopStyleColor(2);
}

void CUiManager::LoadWorld(CWorld& world)
{
	ImGui::TextColored(ImVec4(1, 1, 0, 1), "Load");

	ImGui::Separator();

	if (ImGui::BeginCombo("World to load", m_worldsToLoad[ISLANDS_WORLD_CURRENT_INDEX].data(), 0))
	{
		for (int n = 0; n < m_worldsToLoad.size(); n++)
		{
			const bool is_selected = (ISLANDS_WORLD_CURRENT_INDEX == n);
			if (ImGui::Selectable(m_worldsToLoad[n].data(), is_selected))
			{
				ISLANDS_WORLD_CURRENT_INDEX = n;
			}

			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndCombo();
	}

	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.3f, 0.0f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.6f, 0.5f, 0.0f, 1.0f));
	if (ImGui::Button("Load world (from .txt file)"))
	{
		world.Load(m_worldsToLoad[ISLANDS_WORLD_CURRENT_INDEX]);
	}
	ImGui::PopStyleColor(2);
}

void CUiManager::DetectIslands(CWorld& world)
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.2f, 0.7f, 0.2f, 1.0f));
	ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.9f, 0.2f, 1.0f));
	if (ImGui::Button("Detect islands"))
	{
		ISLANDS_NUMBER_OF_ISLANDS = world.DetectIslands();
	}
	ImGui::PopStyleColor(2);

	ImGui::Text("Number of islands: %d", ISLANDS_NUMBER_OF_ISLANDS);
}

void CUiManager::GetWorldsToLoad()
{
	m_worldsToLoad.clear();

	for (const auto& entry : std::filesystem::directory_iterator(std::string(WORLDS_PATH)))
	{
		const auto& path = entry.path();
		if (path.extension() == ".txt")
		{
			m_worldsToLoad.emplace_back(path.filename().string());
		}
	}
}
