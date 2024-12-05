#include "Collision.h"
#include "CircuitCalculations.h"
#include "StateHelper.h"
#include "raylib.h"

void Collision::checkNodes(std::list<NodeObject>& nodes, StatusText& statusText) {
    if (m_hoverTriggerd)
        return;

    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), m_camera);

    for (auto& node : nodes) {
        if (CheckCollisionPointCircle(mousePos, node.pos, UI::cellSize / 3.0f)) {
            m_hoverTriggerd = true;

            if (node.solved) {
                statusText.clearText();
                statusText.addText("V = " + getDisplayText(node.value, DrawState::VOLTAGE_SOURCE));
                statusText.addText(", N = " + toString(node.graphNode.value));

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

void Collision::checkCircuitElements(std::list<CircuitElement>& circuitElements, std::list<NodeObject>& nodes, Input& input, StatusText& statusText) {
    if (m_hoverTriggerd)
        return;

    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), m_camera);

    for (auto it = circuitElements.begin(); it != circuitElements.end(); it++) {
        auto& circuitElement = *it;
        if (CheckCollisionPointLine(mousePos, circuitElement.startNode->pos, circuitElement.endNode->pos, UI::circuitElementHeight)) {
            m_hoverTriggerd = true;
            circuitElement.setOpacity(0.6f);

            if (IsKeyPressed(KEY_DELETE)) {
                deleteElement(it, circuitElements, nodes);
                break;
            }

            const bool invalidDrawStates = circuitElement.state != DrawState::WIRE && circuitElement.state != DrawState::GROUND;

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && invalidDrawStates && !m_currentElement.isDrawing()) {
                input.assign(it);
                break;
            }

            if (circuitElement.startNode->solved && circuitElement.endNode->solved) {
                statusText.clearText();
                statusText.addText("\xCE\x94V=" + getDisplayText(calculateVoltageDiff(circuitElement), DrawState::VOLTAGE_SOURCE));
                statusText.addText(", I= " + getDisplayText(circuitElement.current, DrawState::CURRENT_SOURCE));
            }


            break;
        }
    }
}


void Collision::checkGridNodes(std::list<NodeObject>& nodes, Vector2& hoverdCircle) {
    if (m_hoverTriggerd)
        return; 

    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), m_camera);

    for (int i = 0; i <= UI::slices; ++i) {
        for (int j = 0; j <= UI::slices * 0.5; ++j) {
            Vector2 checkCircleCenter = Vector2{ UI::cellSize * i, UI::cellSize * j };
            if (CheckCollisionPointCircle(mousePos, checkCircleCenter, UI::cellSize / 3.0f)) {
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