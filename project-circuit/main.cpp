#include <iostream>
#include <vector>
#include <list>
#include "raylib.h"
#include "raymath.h"

#include "Graph.h"
#include "Circuit.h"

inline namespace Constants {
    inline constexpr int width = 1200;
    inline constexpr int height = 800;
    inline constexpr float cellSize = 40;
    inline constexpr float lineNum = width / cellSize - 1;

    Color GRID_COLOR{ 212, 212, 212, 255 };
    Color WIRE_COLOR{ 34,92,137,255 };


    enum DrawState {
        RESISTOR_DRAW,
        WIRE_DRAW,
        VOLTAGE_SOURCE_DRAW,
        CURRENT_SOURCE_DRAW
    };
}

struct NodeObject;

struct CircuitElement {
    DrawState state{ RESISTOR_DRAW };
    NodeObject* startNode{ nullptr };
    NodeObject* endNode{ nullptr };
};


struct NodeObject
{
    Vector2 pos{};
    Graph::Node graphNode;
    bool solved{ false };
};


void drawElement(Texture2D texture, Rectangle sourceTextureRec, const CircuitElement& circuitElement);
void drawWire(const CircuitElement& circuitElement);
std::vector<Number> SolveCircuit(std::list<NodeObject>& nodes, std::list<CircuitElement>& circuitElements);

int main(void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    InitWindow(width, height, "Circuit Simulator");


    SetTargetFPS(60);
    int codePointCount = 0;
    Font font = LoadFont("./dejavu.fnt");
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    Texture2D resistor = LoadTexture("./Resistor.png");
    Texture2D voltageSource = LoadTexture("./Voltage Source.png");
    Texture2D currentSource = LoadTexture("./Current Source.png");


    Rectangle sourceTexture = { 0.0f, 0.0f, (float)resistor.width, (float)resistor.height };
    Rectangle destTextureRec = { 0.0f, 0.0f, cellSize, cellSize };

    std::list<NodeObject> nodes{};
    std::list<CircuitElement> circuitElements{};
    bool clickedHover = false;


    Vector2 hoverdCircle{ -1, -1 };
    Vector2 origin{ 0, cellSize / 2.0f };

    CircuitElement currentCircuitElement{};

    bool drawExtentions = false;
    bool drawCircuitElement = false;

    DrawState currentDrawState{ RESISTOR_DRAW };

    int nodeCounter{ 0 };

    Vector2 ElementVector;
    float ElementLength = 0;
    float lineLength = 0;
    float rotation = 0;

    std::vector<Number> solution;
    bool showStatus = false;
    std::string statusText{};

    int ground = 0;

    while (!WindowShouldClose()) {
        hoverdCircle = { -1, -1 };
        clickedHover = false;
        showStatus = false;

        if (IsKeyPressed(KEY_R)) currentDrawState = RESISTOR_DRAW;
        else if (IsKeyPressed(KEY_V)) currentDrawState = VOLTAGE_SOURCE_DRAW;
        else if (IsKeyPressed(KEY_C)) currentDrawState = CURRENT_SOURCE_DRAW;
        else if (IsKeyPressed(KEY_W)) currentDrawState = WIRE_DRAW;
        else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_ENTER)) solution = SolveCircuit(nodes, circuitElements);


        for (auto& node : nodes) {
            if (CheckCollisionPointCircle(GetMousePosition(), node.pos, cellSize / 3.0f)) {
                clickedHover = true;
                if (node.solved && solution.size() > getRealNode(node.graphNode.value, ground)) {
                    showStatus = true;
                    float value = node.graphNode.value == ground ? 0.0f : static_cast<float>(solution[getRealNode(node.graphNode.value, ground)]);
                    statusText = "V = " + std::to_string(value) + "V";
                }

                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if (drawCircuitElement) {
                        currentCircuitElement.endNode = &node;
                        currentCircuitElement.state = currentDrawState;
                        currentCircuitElement.endNode->graphNode.edges.push_back(Graph::Edge{ currentCircuitElement.startNode->graphNode, currentDrawState == WIRE_DRAW ? true : false });
                        currentCircuitElement.startNode->graphNode.edges.push_back(Graph::Edge{ currentCircuitElement.endNode->graphNode, currentDrawState == WIRE_DRAW ? true : false });
                        circuitElements.push_back(currentCircuitElement);
                    }
                    else {
                        currentCircuitElement.startNode = &node;
                    }
                    drawCircuitElement = !drawCircuitElement;
                }
                break;
            }
        }

        if (!clickedHover) {
            for (int i = 1; i <= lineNum; ++i) {
                for (int j = 1; j <= lineNum; ++j) {
                    Vector2 checkCircleCenter = Vector2{ cellSize * i, cellSize * j };
                    if (CheckCollisionPointCircle(GetMousePosition(), checkCircleCenter, cellSize / 3.0f)) {
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                            nodes.push_back({ checkCircleCenter, {nodeCounter++} });

                            NodeObject& lastNode = nodes.back();

                            if (drawCircuitElement) {
                                currentCircuitElement.endNode = &lastNode;
                                currentCircuitElement.state = currentDrawState;
                                currentCircuitElement.endNode->graphNode.edges.push_back(Graph::Edge{ currentCircuitElement.startNode->graphNode, currentDrawState == WIRE_DRAW ? true : false });
                                currentCircuitElement.startNode->graphNode.edges.push_back(Graph::Edge{ currentCircuitElement.endNode->graphNode, currentDrawState == WIRE_DRAW ? true : false });
                                circuitElements.push_back(currentCircuitElement);
                            }
                            else {
                                currentCircuitElement.startNode = &lastNode;
                            }

                            drawCircuitElement = !drawCircuitElement;
                            break;
                        }

                        hoverdCircle = checkCircleCenter;
                        break;
                    }

                }
            }
        }


        if (drawCircuitElement) {
            if (currentCircuitElement.startNode) {
                ElementVector = Vector2Subtract(GetMousePosition(), Vector2{ currentCircuitElement.startNode->pos.x, currentCircuitElement.startNode->pos.y });
                ElementLength = Vector2Length(ElementVector);
                lineLength = (Vector2Length(ElementVector) - destTextureRec.width) / 2.0f;
                rotation = 180.0f - (Vector2Angle(Vector2Subtract(Vector2{ currentCircuitElement.startNode->pos.x, currentCircuitElement.startNode->pos.y }, GetMousePosition()), Vector2{ 1,0 }) * 180.0f / PI);
            }

            if (ElementLength > cellSize) {
                destTextureRec.width = cellSize;
                destTextureRec.height = cellSize;
                origin = { 0, destTextureRec.height / 2.0f };

                drawExtentions = true;
                origin.x = -lineLength;
            }
            else {
                destTextureRec.width = ElementLength;
                destTextureRec.height = ElementLength;

                drawExtentions = false;
                origin = { 0, destTextureRec.height / 2.0f };
            }
        }

        BeginDrawing();
        ClearBackground(WHITE);
        for (int i = 1; i <= lineNum; ++i) {
            DrawLineEx(Vector2{ 0,cellSize * i }, Vector2{ width, cellSize * i }, 1.0, GRID_COLOR);
        }

        for (int i = 1; i <= lineNum; ++i) {
            DrawLineEx(Vector2{ cellSize * i, 0 }, Vector2{ cellSize * i, height }, 1.0, GRID_COLOR);
        }

        if (drawCircuitElement && currentDrawState != WIRE_DRAW && currentCircuitElement.startNode) {
            destTextureRec.x = currentCircuitElement.startNode->pos.x;
            destTextureRec.y = currentCircuitElement.startNode->pos.y;

            Texture2D currentTexture;

            switch (currentDrawState)
            {
            case RESISTOR_DRAW:
                currentTexture = resistor;
                break;
            case VOLTAGE_SOURCE_DRAW:
                currentTexture = voltageSource;
                break;
            case CURRENT_SOURCE_DRAW:
                currentTexture = currentSource;
                break;
            default:
                currentTexture = resistor;
                break;
            }

            DrawTexturePro(currentTexture, sourceTexture, destTextureRec, origin, rotation, WHITE);

            if (drawExtentions) {
                DrawLineEx({ destTextureRec.x, destTextureRec.y }, Vector2Add({ destTextureRec.x, destTextureRec.y }, Vector2Scale(Vector2Normalize(ElementVector), lineLength)), 2.0f, WIRE_COLOR);
                DrawLineEx(Vector2Subtract(GetMousePosition(), Vector2Scale(Vector2Normalize(ElementVector), lineLength)), GetMousePosition(), 2.0f, WIRE_COLOR);
            }
        }

        if (drawCircuitElement && currentDrawState == WIRE_DRAW && currentCircuitElement.startNode) {
            DrawLineEx(currentCircuitElement.startNode->pos, GetMousePosition(), 2.0f, WIRE_COLOR);
        }

        for (const auto& circuitElement : circuitElements) {
            switch (circuitElement.state)
            {
            case RESISTOR_DRAW:
                drawElement(resistor, sourceTexture, circuitElement);
                break;
            case VOLTAGE_SOURCE_DRAW:
                drawElement(voltageSource, sourceTexture, circuitElement);
                break;
            case CURRENT_SOURCE_DRAW:
                drawElement(currentSource, sourceTexture, circuitElement);
                break;
            case WIRE_DRAW:
                drawWire(circuitElement);
                break;
            default:
                break;
            }

        }

        if (hoverdCircle.x != -1 && hoverdCircle.y != -1) {
            DrawCircleV(hoverdCircle, 5, GRID_COLOR);
        }

        for (const auto& node : nodes) {
            DrawCircleV(node.pos, 5, WIRE_COLOR);
        }

        if (showStatus) {
            DrawTextEx(font, statusText.c_str(), Vector2{20, 20}, 20, 1, BLACK);
        }

        EndDrawing();
    }

    UnloadTexture(resistor);
    UnloadTexture(currentSource);
    UnloadTexture(voltageSource);

    CloseWindow();
    return 0;
}


void drawElement(Texture2D texture, Rectangle sourceTextureRec, const CircuitElement& circuitElement) {
    if (!circuitElement.startNode || !circuitElement.endNode)
        return;

    Rectangle dest = { 0.0f, 0.0f, cellSize, cellSize };

    dest.x = circuitElement.startNode->pos.x;
    dest.y = circuitElement.startNode->pos.y;

    Vector2 ElementV = Vector2Subtract(circuitElement.endNode->pos, circuitElement.startNode->pos);
    float lineLength = (Vector2Length(ElementV) - dest.width) / 2.0f;

    float ce_rotation = 180.0f - (Vector2Angle(Vector2Subtract(circuitElement.startNode->pos, circuitElement.endNode->pos), Vector2{ 1,0 }) * 180.0f / PI);
    Vector2 ce_origin = { 0, dest.height / 2.0f };

    if (Vector2Length(ElementV) > cellSize) {
        DrawLineEx({ dest.x, dest.y }, Vector2Add({ dest.x, dest.y }, Vector2Scale(Vector2Normalize(ElementV), lineLength)), 2.0f, WIRE_COLOR);
        DrawLineEx(Vector2Subtract(circuitElement.endNode->pos, Vector2Scale(Vector2Normalize(ElementV), lineLength)), circuitElement.endNode->pos, 2.0f, WIRE_COLOR);
        ce_origin.x = -lineLength;
    }

    DrawTexturePro(texture, sourceTextureRec, dest, ce_origin, ce_rotation, WHITE);
}

void drawWire(const CircuitElement& circuitElement) {
    if (!circuitElement.startNode || !circuitElement.endNode)
        return;

    DrawLineEx(circuitElement.startNode->pos, circuitElement.endNode->pos, 2.0f, WIRE_COLOR);
}

std::vector<Number> SolveCircuit(std::list<NodeObject>& nodes, std::list<CircuitElement>& circuitElements) {
    int nodeCounter{ 0 };

    for (auto& node : nodes) {
        node.solved = true;
        if (node.graphNode.visited)
            continue;

        Graph::dfs(node.graphNode, nodeCounter);
        ++nodeCounter;
    }

    Circuit circuit;

    for (auto& circuitElement : circuitElements) {
        int nodei = circuitElement.startNode->graphNode.value;
        int nodej = circuitElement.endNode->graphNode.value;

        switch (circuitElement.state)
        {
        case RESISTOR_DRAW:
            circuit.addElement(std::make_unique<Resistor>(nodei, nodej, 5));
            break;
        case VOLTAGE_SOURCE_DRAW:
            circuit.addElement(std::make_unique<VoltageSource>(nodei, nodej, 5));
            break;
        case CURRENT_SOURCE_DRAW:
            circuit.addElement(std::make_unique<CurrentSource>(nodei, nodej, 5));
            break;
        default:
            break;
        }
    }

    auto solution = circuit.solve();
    auto values = circuit.makeUnkownValuePair(solution);

    std::cout << "Solution: ";
    for (auto& pair : values) {
        std::cout << pair.first << " = " << pair.second << ", ";
    }
    std::cout << '\n';

    std::for_each(nodes.begin(), nodes.end(), [](NodeObject& node) {
        node.graphNode.visited = false;
        });

    return solution;
}

// TODO
// fix the start and end are the same
// render statusText
// hover over elements
// change circuit element value
// refactor
// calculate the voltage diff and current in any element