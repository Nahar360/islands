#include <string>
#include <utility> // for std::pair
#include <vector>

namespace UiSettings
{
int WORLD_COLS = 2;
int WORLD_ROWS = 2;

char WORLD_FILE_NAME_TO_SAVE[255] = ".txt";

std::vector<std::pair<std::vector<int>, std::string>> ISLANDS = {};

int NUM_UNIQUE_ISLANDS = 0;

int WORLD_CURRENT_TYPE = 0;
int WORLD_CURRENT_INDEX = 0;
} // namespace UiSettings
