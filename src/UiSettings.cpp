#include "UiSettings.hpp"

int NUM_FIREFLIES = 2;

float FIREFLIES_RADIUS = 50.0f;
float INFLUENCE_RADIUS = 150.0f;

float MIN_BLINKING_RATE = 1.0f;
float MAX_BLINKING_RATE = 3.0f;

float FIREFLIES_COLOR_INPUT[3] = { 1.0f, 0.0f, 0.0f };

const char* SHOW_LINES_OPTIONS[3] = { "None", "All", "Only neighbours" };
int SHOW_LINES_OPTION = 0;

bool SHOW_INFLUENCE_RADIUS = true;
bool SHOW_VERTICES = false;

float BLINKING_DURATION = 0.1f;

// ----------------------------

int ISLANDS_WORLD_COLS = 2;
int ISLANDS_WORLD_ROWS = 2;
int ISLANDS_TILE_SIZE_PIXELS = 32;

char ISLANDS_WORLD_FILE_NAME_TO_SAVE[255] = ".txt";

int ISLANDS_NUMBER_OF_ISLANDS = 0;

int ISLANDS_WORLD_CURRENT_INDEX = 0;
