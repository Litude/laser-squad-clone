#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Config.hpp>

#define TILESIZE 32
#define MENUSIZE 200
#define TILE_CENTER sf::Vector2f(TILESIZE/2, TILESIZE/2)

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

#define NUM_ANIMATIONS 4

#define MAX_ITEMS 8
#define ITEMS_PER_ROW 4


// Macros for SFML 2.3 and 2.4 compatiblity without depreciation warnings
#if SFML_VERSION_MAJOR == 2 && SFML_VERSION_MINOR <= 3
#define setTextColor setColor
#define setShaderUniform setParameter
#else
#define setTextColor setFillColor
#define setShaderUniform setUniform
#endif

#endif