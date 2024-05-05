#ifndef UI_SETTINGS_HPP
#define UI_SETTINGS_HPP

#include <vector>

namespace UiSettings
{
extern int WORLD_COLS;
extern int WORLD_ROWS;

extern char WORLD_FILE_NAME_TO_SAVE[255];

extern std::vector<std::vector<int>> ISLANDS;

extern int NUM_UNIQUE_ISLANDS;

extern int WORLD_CURRENT_TYPE;
extern int WORLD_CURRENT_INDEX;
} // namespace UiSettings

#endif // UI_SETTINGS_HPP
