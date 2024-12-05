#pragma once
#include "CurrentCircuitElement.h"
#include "StatusText.h"
#include "TextHelper.h"
#include "Input.h"

class Collision {
private:
    bool m_hoverTriggerd{ false };
    CurrentCircuitElement& m_currentElement;
    DrawState& m_currentDrawState;
    Camera2D& m_camera;

public:
    Collision(CurrentCircuitElement& currentElement, DrawState& currentDrawState, Camera2D& camera)
        : m_currentElement(currentElement)
        , m_currentDrawState(currentDrawState)
        , m_hoverTriggerd(false)
        , m_camera(camera)
    {}

    void checkNodes(std::list<NodeObject>& nodes, StatusText& statusText);
    void checkCircuitElements(std::list<CircuitElement>& circuitElements, std::list<NodeObject>& nodes, Input& input, StatusText& statusText);
    void checkGridNodes(std::list<NodeObject>& nodes, Vector2& hoverdCircle);

    void reset() { m_hoverTriggerd = false; }

};
