#include <iostream>
#include <vector>
#include <list>
#include <sstream>
#include "raylib.h"
#include "raymath.h"

#include "Graph.h"
#include "Circuit.h"

inline namespace Constants {
    inline constexpr int width = 1200;
    inline constexpr int height = 800;
    inline constexpr float cellSize = 40;
    inline constexpr float lineNum = width / cellSize - 1;
    inline constexpr float circuitElementHeight = 13.0f;

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
    float value{5.0f};
    float current{0.0f};
};


struct NodeObject
{
    Vector2 pos{};
    Graph::Node graphNode;
    bool solved{ false };
    float value{};
};

void drawElementText(Font font, float value, Vector2 ElementVector, Vector2 endPos, float rotation, DrawState drawState);
void drawElement(Texture2D texture, Font font, Rectangle sourceTextureRec, const CircuitElement& circuitElement);
void drawWire(const CircuitElement& circuitElement);

bool SolveCircuit(std::list<NodeObject>& nodes, std::list<CircuitElement>& circuitElements);

float calculateVoltageDiff(const CircuitElement& circuitElement);

std::string toString(float value);
std::string getUnit(DrawState drawState);




int main(void)
{
    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);

    InitWindow(width, height, "Circuit Simulator");


    SetTargetFPS(60);
    int codePointCount = 0;
    Font font = LoadFont("./dejavu.fnt");
    SetTextureFilter(font.texture, TEXTURE_FILTER_TRILINEAR);

    Texture2D resistor = LoadTexture("./Resistor.png");
    Texture2D voltageSource = LoadTexture("./Voltage Source.png");
    Texture2D currentSource = LoadTexture("./Current Source.png");


    Rectangle sourceTexture = { 0.0f, 0.0f, (float)resistor.width, (float)resistor.height };
    Rectangle destTextureRec = { 0.0f, 0.0f, cellSize, cellSize };

    std::list<NodeObject> nodes{};
    std::list<CircuitElement> circuitElements{};


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
            if (IsKeyPressed(KEY_R)) currentDrawState = RESISTOR_DRAW;
            else if (IsKeyPressed(KEY_V)) currentDrawState = VOLTAGE_SOURCE_DRAW;
            else if (IsKeyPressed(KEY_C)) currentDrawState = CURRENT_SOURCE_DRAW;
            else if (IsKeyPressed(KEY_W)) currentDrawState = WIRE_DRAW;
            else if (IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_ENTER)) solved = SolveCircuit(nodes, circuitElements);

            for (auto& node : nodes) {
                if (CheckCollisionPointCircle(GetMousePosition(), node.pos, cellSize / 3.0f)) {
                    hoverTriggerd = true;
                    if (node.solved) {
                        showStatus = true;
                        statusText = "V = " + toString(node.value) + "V";
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
            for (auto& circuitElement : circuitElements) {
                if (hoverTriggerd)
                    break;

                if (CheckCollisionPointLine(GetMousePosition(), circuitElement.startNode->pos, circuitElement.endNode->pos, circuitElementHeight)) {
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

                        if (circuitElement.state != WIRE_DRAW) {
                            statusText += ", I= " + toString(circuitElement.current) + "A";
                        }
                    }

                    break;
                }
            }


          

            for (int i = 1; i <= lineNum && !hoverTriggerd; ++i) {
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
                        hoverTriggerd = true;
                        break;
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

            drawElementText(font, currentCircuitElement.value, ElementVector, GetMousePosition(), rotation, currentDrawState);

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
                drawElement(resistor, font, sourceTexture, circuitElement);
                break;
            case VOLTAGE_SOURCE_DRAW:
                drawElement(voltageSource, font, sourceTexture, circuitElement);
                break;
            case CURRENT_SOURCE_DRAW:
                drawElement(currentSource, font, sourceTexture, circuitElement);
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

float calculateVoltageDiff(const CircuitElement& circuitElement) {
    switch (circuitElement.state)
    {
    case Constants::RESISTOR_DRAW:
    case Constants::VOLTAGE_SOURCE_DRAW:
    case Constants::CURRENT_SOURCE_DRAW:
        return abs(circuitElement.startNode->value - circuitElement.endNode->value);
    default:
        return 0.0f;
    }
}

std::string toString(float value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string getUnit(DrawState drawState) {
    switch (drawState)
    {
    case Constants::RESISTOR_DRAW:
        return "\xCE\xA9";
    case Constants::VOLTAGE_SOURCE_DRAW:
        return "V";
    case Constants::CURRENT_SOURCE_DRAW:
        return "A";
    default:
        return "";
    }
}

void drawElementText(Font font, float value, Vector2 ElementVector, Vector2 endPos, float rotation, DrawState drawState) {
    if (Vector2Length(ElementVector) < cellSize / 2.0f)
        return;
    std::string text{ toString(value) + getUnit(drawState) };

    bool isInLefSide = (rotation >= 0 && rotation <= 90) || (rotation <= 360 && rotation >= 270);

    int textHeight = 13;
    int textWidth = MeasureText(text.c_str(), textHeight);
    std::cout << rotation << '\n';
    float distance = drawState == RESISTOR_DRAW ? 17.0f + abs(textWidth / 3.0f * sin(DEG2RAD * (rotation))) : 25.0f + abs(textWidth / 3.0f * sin(DEG2RAD * (rotation)));

    Vector2 PerpendicularUnitVector = Vector2Normalize(Vector2{ (!isInLefSide ? -1.0f : 1.0f) * ElementVector.y, (isInLefSide ? -1.0f : 1.0f) * ElementVector.x });
    Vector2 perpendicularElementVector = Vector2Scale(PerpendicularUnitVector, distance);
    Vector2 midpoint = Vector2Subtract(endPos, Vector2Scale(ElementVector,  0.5f));

    Vector2 textPos = Vector2Add(midpoint, perpendicularElementVector);

    textPos.x -= textWidth / 2.0f;
    textPos.y -= textHeight / 2.0f;
  

    DrawTextEx(font, text.c_str(), textPos, textHeight, 1, BLACK);
}

void drawElement(Texture2D texture, Font font, Rectangle sourceTextureRec, const CircuitElement& circuitElement) {
    if (!circuitElement.startNode || !circuitElement.endNode)
        return;

    Rectangle dest = { 0.0f, 0.0f, cellSize, cellSize };

    dest.x = circuitElement.startNode->pos.x;
    dest.y = circuitElement.startNode->pos.y;

    Vector2 ElementV = Vector2Subtract(circuitElement.endNode->pos, circuitElement.startNode->pos);
    float lineLength = (Vector2Length(ElementV) - dest.width) / 2.0f;

    float ce_rotation = 180.0f - (Vector2Angle(Vector2Subtract(circuitElement.startNode->pos, circuitElement.endNode->pos), Vector2{ 1,0 }) * 180.0f / PI);
    Vector2 ce_origin = { 0, dest.height / 2.0f };

    drawElementText(font, circuitElement.value, ElementV, circuitElement.endNode->pos, ce_rotation, circuitElement.state);

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

bool SolveCircuit(std::list<NodeObject>& nodes, std::list<CircuitElement>& circuitElements) {
    int nodeCounter{ 0 };
    int ground{ 0 };

    for (auto& node : nodes) {
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
            circuit.addElement(std::make_unique<Resistor>(nodei, nodej, circuitElement.value));
            break;
        case VOLTAGE_SOURCE_DRAW:
            circuit.addElement(std::make_unique<VoltageSource>(nodei, nodej, circuitElement.value));
            break;
        case CURRENT_SOURCE_DRAW:
            circuit.addElement(std::make_unique<CurrentSource>(nodei, nodej, circuitElement.value));
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
    
    if (solution.size() <= 0)
        return false;



    for (auto& node : nodes) {
        node.graphNode.visited = false;
        node.solved = true;

        if (node.graphNode.value == ground) {
            node.value = 0.0f;
            continue;
        }

        if (solution.size() > getRealNode(node.graphNode.value, ground)) {
            node.value = static_cast<float>(solution[getRealNode(node.graphNode.value, ground)]);
        }
    }

    int extractedVoltageCurrents = 0;
    int unkownNodes = circuit.getUnkownsNumber();

    for (auto& circuitElement : circuitElements) {
        std::size_t solutionIndex;

        switch (circuitElement.state)
        {
        case RESISTOR_DRAW:
            circuitElement.current = calculateVoltageDiff(circuitElement) / circuitElement.value;
            break;

        case VOLTAGE_SOURCE_DRAW:
            solutionIndex = static_cast<std::size_t>(unkownNodes + extractedVoltageCurrents);

            if (solution.size() > solutionIndex) {
                circuitElement.current = solution[solutionIndex];
                ++extractedVoltageCurrents;
            }

            break;

        case CURRENT_SOURCE_DRAW:
            circuitElement.current = circuitElement.value;
            break;
        }
    }

    return true;
}

// TODO
// refactor
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
// make blinking animation when editing a circuits element 
// make insert mode which allows the insertion of the value of all the circuit elements
// add a direction indecator for resistors and voltage sources indicating which direction the current is flowing