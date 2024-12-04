#ifndef UI_CONSTANTS_H
#define UI_CONSTANTS_H
#include "raylib.h"
#include <array>

namespace UI {
    inline constexpr int width = 1000;
    inline constexpr int height = 600;
    inline constexpr float cellSize = 40;
    inline constexpr float lineNum = width / cellSize - 1;
    inline constexpr float circuitElementHeight = 13.0f;

    inline Color GRID_COLOR{ 212, 212, 212, 255 };
    inline Color WIRE_COLOR{ 34,92,137,255 };
}

 enum class DrawState{
    RESISTOR,
    VOLTAGE_SOURCE,
    CURRENT_SOURCE,
    GROUND,
    WIRE
};

// WIRE  doesn't have a texture and it in this case represents the count of elements
using TexturesArray = std::array<Texture2D, static_cast<std::size_t>(DrawState::WIRE)>;

#endif // !UI_CONSTATNS_H
