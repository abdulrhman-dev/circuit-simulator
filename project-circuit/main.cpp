#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <cassert>

#include "raylib.h"
#include "raymath.h"

#include "Circuit.h"
#include "Graph.h"
#include "CircuitElement.h"
#include "CurrentCircuitElement.h"
#include "StatusText.h"
#include "NodeObject.h"
#include "Collision.h"
#include "Input.h"
#include "CircuitCalculations.h"
#include "StateHelper.h"
#include "Keyboard.h"

int main(void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    //SetConfigFlags(FLAG_WINDOW_RESIZABLE);

    InitWindow(UI::width, UI::height, "Circuit Solver");

    SetTargetFPS(60);

    Font font = LoadFont("./assets/dejavu.fnt");
    SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);

    TexturesArray textures{ 
        LoadTexture("./assets/Resistor.png"),
        LoadTexture("./assets/Voltage Source.png"),
        LoadTexture("./assets/Current Source.png"),
        LoadTexture("./assets/Ground.png")
    };

    for (auto& texture : textures)
        SetTextureFilter(texture, TEXTURE_FILTER_POINT);

    // WIRE  doesn't have a texture and it, in this case, represents the count of elements
    assert(textures.size() == static_cast<std::size_t>(DrawState::WIRE));

    std::list<NodeObject> nodes{};
    std::list<CircuitElement> circuitElements{};

    CurrentCircuitElement currentElement{ circuitElements };
    DrawState currentDrawState{ DrawState::RESISTOR };
    StatusText statusText(font);
    Input input(font, circuitElements);

    Collision collision(currentElement,currentDrawState, statusText, input);

    Vector2 hoverdCircle{ -1, -1 };

    while (!WindowShouldClose())     {
        hoverdCircle = { -1, -1 };
        statusText.reset();
        collision.reset();
        
        if (!input.isInputMode()) {
            Keyboard::handleModeSwitch(currentDrawState);
            Keyboard::handleBeginInsertMode(circuitElements, input);
            Keyboard::handleUndo(circuitElements, nodes, input, currentElement.isDrawing());
            Keyboard::handleSolve(circuitElements, nodes, statusText);

            collision.checkNodes(nodes);
            collision.checkCircuitElements(circuitElements, nodes);
            collision.checkGridNodes(nodes, hoverdCircle);

            currentElement.update(currentDrawState, nodes);
        }
        else
            input.handle();
        
        BeginDrawing();
        ClearBackground(WHITE);

        for (int i = 1; i <= UI::lineNum; ++i) 
            DrawLineEx(Vector2{ 0,UI::cellSize * i }, Vector2{ UI::width, UI::cellSize * i }, 1.0, UI::GRID_COLOR);

        for (int i = 1; i <= UI::lineNum; ++i)
            DrawLineEx(Vector2{ UI::cellSize * i, 0 }, Vector2{ UI::cellSize * i, UI::height }, 1.0, UI::GRID_COLOR);
        
        currentElement.draw(font, textures);

        for (auto& circuitElement : circuitElements)
            circuitElement.draw(font, textures);
 
        if (hoverdCircle.x != -1 && hoverdCircle.y != -1) 
            DrawCircleV(hoverdCircle, 5, UI::GRID_COLOR);
  
        for (const auto& node : nodes)
            node.draw();

        statusText.draw();
        input.draw();

        EndDrawing();
    }

    for (auto& texture : textures) 
        UnloadTexture(texture);

    CloseWindow();
}

// TODO
// multiple circuits one canvas
// camera that allows  to zoom in and out
// add a direction indecator for resistors and voltage sources indicating which direction the current is flowing
// add a way to export the solution as an excel file
// add a way to label nodes
// hide text mode
// - show the value when hovering
// - when editing the circuit element value 