#ifndef  OUTPUT_H
#define OUTPUT_H
#include <list>
#include <array>
#include "raylib.h"
#include "raymath.h"

#include "UIConstants.h"
#include "CircuitStamp.h"
#include "Graph.h"


struct NodeObject
{
    Vector2 pos{};
    Graph::Node graphNode{};
    bool solved{ false };
    float value{};

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

class CircuitElement {
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


class CurrentCircuitElement : public CircuitElement {
private:
    bool drawingElement{ false };
    std::list<CircuitElement>& m_circuitElements;
public:
    CurrentCircuitElement(std::list<CircuitElement>& circuitElements)
        : m_circuitElements(circuitElements)
    {}

    int isDrawing() const { return drawingElement; }
    void addNode(NodeObject& node, DrawState currDrawState);
    void update(DrawState currState);
    void reCalculateRenderInfo(Vector2 endPos);
    void draw(Font font, TexturesArray& textures);
    void reset();
};


class StatusText {
private:
    bool m_showStatus = false;
    std::string m_statusText{};
    Font m_font;

public:
    StatusText(Font font)
       : m_font(font)
    {}

    void addText(const std::string& text) { 
        m_showStatus = true;
        m_statusText += text;
    }

    void clearText() { m_statusText = ""; }

    void reset() { m_showStatus = false; }

    void draw() {
        if (m_showStatus) {
            DrawTextEx(m_font, m_statusText.c_str(), Vector2{ 20, 20 }, 20, 1, BLACK);
        }
    }
};

#endif // ! OUTPUT_H