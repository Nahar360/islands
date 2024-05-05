#include "UiManager.hpp"

#include <algorithm> // for std::sort
#include <filesystem>
#include <string>

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

#include "GlobalSettings.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include "UiSettings.hpp"


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

    UpdateBackgroundColor();

    // -------------------------
    ImGui::Separator();
    ImGui::Separator();
    // -------------------------

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "World settings");

    ImGui::TextColored(ImVec4(1, 1, 0.5, 1), "Create World");

    InitialiseWorld(world);
    ImGui::SameLine();
    InitialiseRandomWorld(world);

    ImGui::Separator();

    ImGui::TextColored(ImVec4(1, 1, 0.5, 1), "Edit World");
    EditWorld(world);

    ClearWorld(world);
    ImGui::SameLine();
    PrintWorldRepresentation(world);

    // -------------------------
    ImGui::Separator();
    ImGui::Separator();
    // -------------------------

    SaveWorld(world);
    ImGui::Separator();
    LoadWorld(world);

    // -------------------------
    ImGui::Separator();
    ImGui::Separator();
    // -------------------------

    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Islands");

    ShowIslandsSummary();
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
    if (ImGui::InputText("Window title", GlobalSettings::WINDOW_TITLE, 255))
    {
        window.setTitle(GlobalSettings::WINDOW_TITLE);
    }
}

void CUiManager::ShowFPS(float fps)
{
    ImGui::Text("FPS: %f", fps);
}

void CUiManager::UpdateMousePosition(sf::RenderWindow& window)
{
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    if (mousePos.x >= 0 && mousePos.x <= GlobalSettings::WINDOW_WIDTH && mousePos.y >= 0 &&
        mousePos.y <= GlobalSettings::WINDOW_HEIGHT)
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
    if (ImGui::ColorEdit3("Background color", GlobalSettings::BACKGROUND_COLOR_INPUT))
    {
        GlobalSettings::BACKGROUND_COLOR.r = static_cast<sf::Uint8>(GlobalSettings::BACKGROUND_COLOR_INPUT[0] * 255.f);
        GlobalSettings::BACKGROUND_COLOR.g = static_cast<sf::Uint8>(GlobalSettings::BACKGROUND_COLOR_INPUT[1] * 255.f);
        GlobalSettings::BACKGROUND_COLOR.b = static_cast<sf::Uint8>(GlobalSettings::BACKGROUND_COLOR_INPUT[2] * 255.f);
    }
}

void CUiManager::InitialiseWorld(CWorld& world)
{
    ImGui::InputInt("No. cols", &UiSettings::WORLD_COLS);
    ImGui::InputInt("No. rows", &UiSettings::WORLD_ROWS);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.1f, 0.4f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.1f, 0.6f, 0.1f, 1.0f));
    if (ImGui::Button("Init. world"))
    {
        ClearWorldAndUI(world);
        world.InitTiles(UiSettings::WORLD_COLS, UiSettings::WORLD_ROWS);

        // Automatically detect islands (and unique ones) when initialising world
        DetectIslands(world);
    }
    ImGui::PopStyleColor(2);
}

void CUiManager::InitialiseRandomWorld(CWorld& world)
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.7f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.9f, 0.4f, 1.0f));
    if (ImGui::Button("Init. random world"))
    {
        ClearWorldAndUI(world);
        world.InitTilesRandom();
        
        // Automatically detect islands (and unique ones) when initialising world
        DetectIslands(world);
    }
    ImGui::PopStyleColor(2);
}

void CUiManager::EditWorld(CWorld& world)
{
    if (ImGui::BeginCombo("Add (types)", m_tileTypesAvailable[UiSettings::WORLD_CURRENT_TYPE].data(), 0))
    {
        for (int n = 0; n < m_tileTypesAvailable.size(); n++)
        {
            const bool is_selected = (UiSettings::WORLD_CURRENT_TYPE == n);
            if (ImGui::Selectable(m_tileTypesAvailable[n].data(), is_selected))
            {
                UiSettings::WORLD_CURRENT_TYPE = n;
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
    if (ImGui::ArrowButton("##leftCol", ImGuiDir_Left))
    {
        world.RemoveColumn();
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("##rightCol", ImGuiDir_Right))
    {
        world.AddColumn(UiSettings::WORLD_CURRENT_TYPE);
    }

    ImGui::TextColored(ImVec4(0.5, 0.5, 0.5, 1), "Rows");
    ImGui::SameLine();
    if (ImGui::ArrowButton("##leftRow", ImGuiDir_Left))
    {
        world.RemoveRow();
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("##rightRow", ImGuiDir_Right))
    {
        world.AddRow(UiSettings::WORLD_CURRENT_TYPE);
    }
}

void CUiManager::ClearWorld(CWorld& world)
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.9f, 0.2f, 0.1f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.2f, 0.1f, 1.0f));
    if (ImGui::Button("Clear world"))
    {
        ClearWorldAndUI(world);
    }
    ImGui::PopStyleColor(2);
}

void CUiManager::PrintWorldRepresentation(CWorld& world)
{
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.4f, 0.1f, 0.6f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.4f, 0.3f, 0.6f, 1.0f));
    if (ImGui::Button("Print world to console"))
    {
        world.PrintRepresentation();
    }
    ImGui::PopStyleColor(2);
}

void CUiManager::SaveWorld(CWorld& world)
{
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Save");

    ImGui::InputText("World to save", UiSettings::WORLD_FILE_NAME_TO_SAVE, 255);

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.8f, 0.5f, 0.2f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.8f, 0.7f, 0.2f, 1.0f));
    if (ImGui::Button("Save world (to .txt file)"))
    {
        const std::string worldFileNameToSave = std::string(UiSettings::WORLD_FILE_NAME_TO_SAVE);
        if (worldFileNameToSave != ".txt")
        {
            const bool saved = world.Save(worldFileNameToSave);
            if (saved)
            {
                GetWorldsToLoad();
            }
        }
    }
    ImGui::PopStyleColor(2);
}

void CUiManager::LoadWorld(CWorld& world)
{
    ImGui::TextColored(ImVec4(1, 1, 0, 1), "Load");

    if (ImGui::BeginCombo("World to load", m_worldsToLoad[UiSettings::WORLD_CURRENT_INDEX].data(), 0))
    {
        for (int n = 0; n < m_worldsToLoad.size(); n++)
        {
            const bool is_selected = (UiSettings::WORLD_CURRENT_INDEX == n);
            if (ImGui::Selectable(m_worldsToLoad[n].data(), is_selected))
            {
                UiSettings::WORLD_CURRENT_INDEX = n;
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
        ClearWorldAndUI(world);
        world.Load(m_worldsToLoad[UiSettings::WORLD_CURRENT_INDEX]);

        // Automatically detect islands (and unique ones) when loading world
        DetectIslands(world);
    }
    ImGui::PopStyleColor(2);
}

void CUiManager::ShowIslandsSummary()
{
    ImGui::Text("Number of islands: %zu", UiSettings::ISLANDS.size());
    for (int i = 0; i < UiSettings::ISLANDS.size(); i++)
    {
        std::string islandLog = "#" + std::to_string(i + 1) + ": { ";
        for (int j = 0; j < UiSettings::ISLANDS[i].size(); j++)
        {
            islandLog += std::to_string(UiSettings::ISLANDS[i][j]);
            if (j != UiSettings::ISLANDS[i].size() - 1)
            {
                islandLog += ", ";
            }
        }
        islandLog += " }";

        ImGui::BulletText("%s", islandLog.c_str());
    }

    ImGui::Text("Number of unique islands: %d", UiSettings::NUM_UNIQUE_ISLANDS);
}

void CUiManager::ClearWorldAndUI(CWorld& world)
{
    world.Clear();
    UiSettings::ISLANDS.clear();
    UiSettings::NUM_UNIQUE_ISLANDS = 0;
}

void CUiManager::GetWorldsToLoad()
{
    m_worldsToLoad.clear();

    for (const auto& entry : std::filesystem::directory_iterator(std::string(GlobalSettings::WORLDS_PATH)))
    {
        const auto& path = entry.path();
        if (path.extension() == ".txt")
        {
            m_worldsToLoad.emplace_back(path.filename().string());
        }
    }

    // Sort world files alphabetically
    std::sort(
        m_worldsToLoad.begin(),
        m_worldsToLoad.end(),
        [](const std::string& a, const std::string& b) -> bool { return a < b; });
}

void CUiManager::DetectIslands(CWorld& world)
{
    UiSettings::ISLANDS = world.DetectIslands();
    UiSettings::NUM_UNIQUE_ISLANDS = world.GetNumUniqueIslands();
}
