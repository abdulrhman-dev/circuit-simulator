#ifndef  OUTPUT_H
#define OUTPUT_H
#include <list>
#include <array>
#include "raylib.h"
#include "raymath.h"

#include "UIConstants.h"
#include "CircuitStamp.h"
#include "TextHelper.h"
#include "Graph.h"


struct NodeObject
{
    Vector2 pos{};
    Graph::Node graphNode{};
    bool solved{ false };
    float value{};

    NodeObject(Vector2& pos_)
        : pos(pos_)
    {}

    void addEdge(NodeObject* node, DrawState& state) {
        graphNode.edges.push_back(Graph::Edge{ node->graphNode, state == DrawState::WIRE ? true : false });
    }

    void draw() const {
        DrawCircleV(pos, 5, UI::WIRE_COLOR);
    }
};

struct CircuitElementRenderInfo {
    Vector2 ElementVector{ 0, 0 };
    Vector2 origin{ 0, UI::cellSize / 2.0f };
    Rectangle destTextureRec{ 0.0f, 0.0f, UI::cellSize, UI::cellSize };
    float ElementLength{ 0 };
    float lineLength  { 0 };
    float rotation  { 0 };
    bool drawExtentions  { false };
};

struct CircuitElement {
public:
    DrawState state { DrawState::RESISTOR };
    NodeObject* startNode{ nullptr };
    NodeObject* endNode{ nullptr };
    float value{ 5.0f };
    float current{ 0.0f };

    void draw(Font font, TexturesArray& textures);
    void drawElement(Vector2 endPos, Font font, TexturesArray& textures);
    void drawElementText(Font font, Vector2 endPos);
protected:
    CircuitElementRenderInfo RenderInfo{};
};


struct CurrentCircuitElement : public CircuitElement {
private:
    bool drawingElement{ false };
public:
    void addNode(NodeObject& node, DrawState currDrawState, std::list<CircuitElement>& circuitElements);
    void update(DrawState currState);
    void reCalculateRenderInfo(Vector2 endPos);
    void draw(Font font, TexturesArray& textures);
    void reset();
};

namespace Collesion {

}
#endif // ! OUTPUT_H