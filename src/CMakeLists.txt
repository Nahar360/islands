add_executable(islands
  GlobalSettings.cpp
  GlobalSettings.hpp
  Game.cpp
  Game.hpp
  Tile.cpp
  Tile.hpp
  TileType.hpp
  UiManager.cpp
  UiManager.hpp
  UiSettings.cpp
  UiSettings.hpp
  World.cpp
  World.hpp
  main.cpp
)

target_link_libraries(islands PRIVATE ImGui-SFML::ImGui-SFML)

# Copy DLLs needed for runtime on Windows
if(WIN32)
  if (BUILD_SHARED_LIBS)
    add_custom_command(TARGET islands POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:ImGui-SFML::ImGui-SFML>
          $<TARGET_FILE:sfml-graphics>
          $<TARGET_FILE:sfml-window>
          $<TARGET_FILE:sfml-system>
          $<TARGET_FILE_DIR:islands>)
  endif()
endif()
