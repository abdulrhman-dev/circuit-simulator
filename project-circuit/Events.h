#ifndef EVENTS_H
#define EVENTS_H
#include "Output.h"
#include "TextHelper.h"
#include "Input.h"

class Events {
private:
    bool m_hoverTriggerd{ false };
    CurrentCircuitElement& m_currentElement;
    DrawState& m_currentDrawState;
    StatusText& m_statusText;
    Input& m_input;

public:
    Events(CurrentCircuitElement& currentElement, DrawState& currentDrawState, StatusText& statusText, Input& input)
        : m_currentElement(currentElement)
        , m_currentDrawState(currentDrawState)
        , m_statusText(statusText)
        , m_hoverTriggerd(false)
        , m_input(input)
    {}

    void checkNodes(std::list<NodeObject>& nodes);
    void checkCircuitElements(std::list<CircuitElement>& circuitElements, std::list<NodeObject>& nodes);
    void checkGridNodes(std::list<NodeObject>& nodes, Vector2& hoverdCircle);

    void reset() { m_hoverTriggerd = false; }

};
#endif // !EVENTS_H
