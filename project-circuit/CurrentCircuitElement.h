#pragma once
#include "CircuitElement.h"

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
    void addNode(Vector2 pos, DrawState currDrawState);
    void update(DrawState currState, std::list<NodeObject>& nodes);
    void reCalculateRenderInfo(Vector2 endPos);
    void draw(Font font, TexturesArray& textures);
    void reset();
};
