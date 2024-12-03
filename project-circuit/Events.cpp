#include "Events.h"
#include "CircuitCalculations.h"
#include "StateHelper.h"
#include "raylib.h"

void Events::checkNodes(std::list<NodeObject>& nodes) {
    if (m_hoverTriggerd)
        return;

    for (auto& node : nodes) {
        if (CheckCollisionPointCircle(GetMousePosition(), node.pos, UI::cellSize / 3.0f)) {
            m_hoverTriggerd = true;

            if (node.solved) {
                m_statusText.clearText();
                m_statusText.addText("V = " + getDisplayText(node.value, DrawState::VOLTAGE_SOURCE));
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && m_currentDrawState == DrawState::GROUND && m_currentElement.startNode) {
                m_currentElement.addNode(node.pos, m_currentDrawState);
            }else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                m_currentElement.addNode(node, m_currentDrawState);
            }

            break;
        }
    }


}

void Events::checkCircuitElements(std::list<CircuitElement>& circuitElements, std::list<NodeObject>& nodes) {
    if (m_hoverTriggerd)
        return;

    for (auto it = circuitElements.begin(); it != circuitElements.end(); it++) {
        auto& circuitElement = *it;
        if (CheckCollisionPointLine(GetMousePosition(), circuitElement.startNode->pos, circuitElement.endNode->pos, UI::circuitElementHeight)) {
            m_hoverTriggerd = true;
            circuitElement.setOpacity(0.6f);

            if (IsKeyPressed(KEY_DELETE)) {
                deleteElement(it, circuitElements, nodes);
                break;
            }

            const bool invalidDrawStates = circuitElement.state != DrawState::WIRE && circuitElement.state != DrawState::GROUND;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && invalidDrawStates && !m_currentElement.isDrawing()) {
                m_input.assign(it);
                break;
            }

            if (circuitElement.startNode->solved && circuitElement.endNode->solved) {
                m_statusText.clearText();
                m_statusText.addText("\xCE\x94V=" + getDisplayText(calculateVoltageDiff(circuitElement), DrawState::VOLTAGE_SOURCE));
                m_statusText.addText(", I= " + getDisplayText(circuitElement.current, DrawState::CURRENT_SOURCE));
            }


            break;
        }
    }
}


void Events::checkGridNodes(std::list<NodeObject>& nodes, Vector2& hoverdCircle) {
    if (m_hoverTriggerd)
        return; 

    for (int i = 1; i <= UI::lineNum; ++i) {
        for (int j = 1; j <= UI::lineNum; ++j) {
            Vector2 checkCircleCenter = Vector2{ UI::cellSize * i, UI::cellSize * j };
            if (CheckCollisionPointCircle(GetMousePosition(), checkCircleCenter, UI::cellSize / 3.0f)) {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                    if (m_currentDrawState == DrawState::GROUND && m_currentElement.startNode) {
                        m_currentElement.addNode(checkCircleCenter, m_currentDrawState);
                    }
                    else if(m_currentDrawState != DrawState::GROUND){
                        nodes.push_back({ checkCircleCenter });
                        m_currentElement.addNode(nodes.back(), m_currentDrawState);
                    }

                    break;
                }

                hoverdCircle = checkCircleCenter;
                m_hoverTriggerd = true;
                break;
            }

        }
    }
}