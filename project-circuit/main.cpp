#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <cassert>

#include "raylib.h"
#include "raymath.h"
#include "rlgl.h"

#include "Circuit.h"
#include "Graph.h"

#include "CircuitElement.h"
#include "CurrentCircuitElement.h"
#include "NodeObject.h"
#include "StatusText.h"

#include "Collision.h"
#include "Input.h"
#include "Keyboard.h"

#include "StateHelper.h"
#include "CircuitCalculations.h"

int main(void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);

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

    Camera2D camera = { 0 };
    camera.zoom = 1.0f;

    CurrentCircuitElement currentElement{ circuitElements };
    DrawState currentDrawState{ DrawState::RESISTOR };
    StatusText statusText(font);
    Input input(font, circuitElements);

    Collision collision(currentElement, currentDrawState, camera);

    Vector2 hoverdCircle{ -1, -1 };

    while (!WindowShouldClose())     {
        hoverdCircle = { -1, -1 };
        statusText.reset();
        collision.reset();
        
        if (!input.isInputMode()) {
            Keyboard::handleCameraActions(camera);
            Keyboard::handleModeSwitch(currentDrawState);
            Keyboard::handleBeginInsertMode(circuitElements, input);
            Keyboard::handleUndo(circuitElements, nodes, input, currentElement.isDrawing());
            Keyboard::handleSolve(circuitElements, nodes, statusText);

            collision.checkNodes(nodes, statusText);
            collision.checkCircuitElements(circuitElements, nodes, input, statusText);
            collision.checkGridNodes(nodes, hoverdCircle);

            currentElement.update(camera, currentDrawState, nodes);
        }
        else
            input.handle();
        
        BeginDrawing();
        ClearBackground(WHITE);

        BeginMode2D(camera);

        rlPushMatrix();
            rlTranslatef(0.5 * UI::slices * UI::cellSize, 0.5 * UI::slices * UI::cellSize, 0);
            rlRotatef(90, 1, 0, 0);
            DrawGrid(UI::slices, UI::cellSize);
        rlPopMatrix();

        currentElement.draw(camera, font, textures);

        for (auto& circuitElement : circuitElements)
            circuitElement.draw(font, textures);
 
        if (hoverdCircle.x != -1 && hoverdCircle.y != -1) 
            DrawCircleV(hoverdCircle, 5, UI::GRID_COLOR);
  
        for (const auto& node : nodes)
            node.draw();

        EndMode2D();

        statusText.draw();
        input.draw();

        EndDrawing();
    }

    for (auto& texture : textures) 
        UnloadTexture(texture);

    CloseWindow();
}

// TODO
// camera that allows  to zoom in and out
// add a direction indecator for resistors and voltage sources indicating which direction the current is flowing
// add a way to export the solution as an excel file
// add a way to label nodes
// hide text mode
// - show the value when hovering
// - when editing the circuit element value 