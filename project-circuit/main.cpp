#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include <cassert>

#include "raylib.h"
#include "raymath.h"

#include "Circuit.h"
#include "Graph.h"
#include "Output.h"
#include "CircuitCalculations.h"

bool SolveCircuit(std::list<NodeObject>& nodes, std::list<CircuitElement>& circuitElements);

float calculateVoltageDiff(const CircuitElement& circuitElement);


int main(void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    InitWindow(UI::width, UI::height, "Circuit Simulator");

    SetTargetFPS(60);

    Font font = LoadFont("./dejavu.fnt");
    SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);

    TexturesArray textures{ LoadTexture("./Resistor.png"), LoadTexture("./Voltage Source.png"),  LoadTexture("./Current Source.png") };

    // WIRE  doesn't have a texture and it in this case represents the count of elements
    assert(textures.size() == static_cast<std::size_t>(DrawState::WIRE));


    std::list<NodeObject> nodes{};
    std::list<CircuitElement> circuitElements{};


    Vector2 hoverdCircle{ -1, -1 };
    Vector2 origin{ 0,UI::cellSize / 2.0f };

    CurrentCircuitElement currentElement{};

    DrawState currentDrawState{ DrawState::RESISTOR };


    bool solved = false;
    bool showStatus = false;
    std::string statusText{};

    int ground = 0;

    bool hoverTriggerd = false;

    bool inputMode = false;
    std::string input{};
    CircuitElement* inputCircuitElement{nullptr};


    while (!WindowShouldClose()) {
        hoverdCircle = { -1, -1 };
        hoverTriggerd = false;
        showStatus = false;

        
        if (!inputMode) {
            if (IsKeyPressed(KEY_R)) currentDrawState = DrawState::RESISTOR;
            else if (IsKeyPressed(KEY_V)) currentDrawState = DrawState::VOLTAGE_SOURCE;
            else if (IsKeyPressed(KEY_C)) currentDrawState = DrawState::CURRENT_SOURCE;
            else if (IsKeyPressed(KEY_W)) currentDrawState = DrawState::WIRE;
            else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_ENTER)) solved = SolveCircuit(nodes, circuitElements);

            for (auto& node : nodes) {
                if (CheckCollisionPointCircle(GetMousePosition(), node.pos, UI::cellSize / 3.0f)) {
                    hoverTriggerd = true;

                    if (node.solved) {
                        showStatus = true;
                        statusText = "V = " + toString(node.value) + "V";
                    }

                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        currentElement.addNode(node, currentDrawState, circuitElements);
                    }
                    break;
                }
            }

            for (auto& circuitElement : circuitElements) {
                if (hoverTriggerd)
                    break;

                if (CheckCollisionPointLine(GetMousePosition(), circuitElement.startNode->pos, circuitElement.endNode->pos, UI::circuitElementHeight)) {
                    hoverTriggerd = true;
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                        inputMode = true;
                        inputCircuitElement = &circuitElement;
                        //input = toString(inputCircuitElement->value);
                        break;
                    }

                    if (circuitElement.startNode->solved && circuitElement.endNode->solved) {
                        showStatus = true;
                        statusText = "\xCE\x94V=" + toString(calculateVoltageDiff(circuitElement)) + "V";

                        if (circuitElement.state != DrawState::WIRE) {
                            statusText += ", I= " + toString(circuitElement.current) + "A";
                        }
                    }

                    break;
                }
            }


            for (int i = 1; i <= UI::lineNum && !hoverTriggerd; ++i) {
                for (int j = 1; j <= UI::lineNum; ++j) {
                    Vector2 checkCircleCenter = Vector2{ UI::cellSize * i, UI::cellSize * j };
                    if (CheckCollisionPointCircle(GetMousePosition(), checkCircleCenter, UI::cellSize / 3.0f)) {
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            nodes.push_back({ checkCircleCenter });

                            currentElement.addNode(nodes.back(), currentDrawState, circuitElements);

                            break;
                        }

                        hoverdCircle = checkCircleCenter;
                        hoverTriggerd = true;
                        break;
                    }

                }
            }

            currentElement.update(currentDrawState);
        }
        else {
            int key = GetCharPressed();

            while (key > 0)
            {
                if ((key >= 48) && (key <= 57) || key == 46)
                {
                    input += static_cast<char>(key);
                }

                key = GetCharPressed(); 
            }

            if (IsKeyPressed(KEY_BACKSPACE))
            {
                if (input.size() > 0) {
                    input.pop_back();
                }
            }

            if (inputCircuitElement && input != "")
                inputCircuitElement->value = std::stof(input);
            else if (input == "")
                inputCircuitElement->value = 0.0f;

            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) {
                inputMode = false;
                input = "";
                inputCircuitElement = nullptr;
            }
        }
        

        BeginDrawing();
        ClearBackground(WHITE);


        for (int i = 1; i <= UI::lineNum; ++i) {
            DrawLineEx(Vector2{ 0,UI::cellSize * i }, Vector2{ UI::width, UI::cellSize * i }, 1.0, UI::GRID_COLOR);
        }

        for (int i = 1; i <= UI::lineNum; ++i) {
            DrawLineEx(Vector2{ UI::cellSize * i, 0 }, Vector2{ UI::cellSize * i, UI::height }, 1.0, UI::GRID_COLOR);
        }
        
        currentElement.draw(font, textures);

        for (auto& circuitElement : circuitElements) {
            circuitElement.draw(font, textures);
        }

        if (hoverdCircle.x != -1 && hoverdCircle.y != -1) {
            DrawCircleV(hoverdCircle, 5, UI::GRID_COLOR);
        }

        for (const auto& node : nodes)
            node.draw();

        if (showStatus) {
            DrawTextEx(font, statusText.c_str(), Vector2{20, 20}, 20, 1, BLACK);
        }

        EndDrawing();
    }

    for (auto& texture : textures) {
        UnloadTexture(texture);
    }

    CloseWindow();
    return 0;
}

// TODO
// refactor
// add CircuitElement in a more effecient way
// fix when the start and end are the same
// put a more high res version of the images
// flip the voltage source direction
// ctrl-z, delete
// multiple circuits one canvas
// camera that allows  to zoom in and out
// display value as  m micro nano, kilo, mega
// allow input of units
// the initial display of an element's value is the original value but when the user starts typing it begins from zero
// handle (.) in input better
// when editing value seperate input from display
// make blinking animation when editing a circuits element 
// make insert mode which allows the insertion of the value of all the circuit elements
// add a direction indecator for resistors and voltage sources indicating which direction the current is flowing
// multiple grounds
// hide text mode
// - show the value when hovering
// - when editing the circuit element value 
