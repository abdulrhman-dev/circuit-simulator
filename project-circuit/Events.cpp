#include "Events.h"

void Events::checkNodes(std::list<NodeObject>& nodes) {
    if (m_hoverTriggerd)
        return;

    for (auto& node : nodes) {
        if (CheckCollisionPointCircle(GetMousePosition(), node.pos, UI::cellSize / 3.0f)) {
            m_hoverTriggerd = true;

            if (node.solved) {
                m_statusText.clearText();
                m_statusText.addText("V = " + toString(node.value) + "V");
            }

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                m_currentElement.addNode(node, m_currentDrawState);
            }

            break;
        }
    }

}

void Events::checkCircuitElements(std::list<CircuitElement>& circuitElements) {
    if (m_hoverTriggerd)
        return;

    for (auto& circuitElement : circuitElements) {
      
        if (CheckCollisionPointLine(GetMousePosition(), circuitElement.startNode->pos, circuitElement.endNode->pos, UI::circuitElementHeight)) {
            m_hoverTriggerd = true;
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
              /*  inputMode = true;
                inputCircuitElement = &circuitElement;*/
                break;
            }

            if (circuitElement.startNode->solved && circuitElement.endNode->solved) {
                m_statusText.clearText();
                m_statusText.addText("\xCE\x94V=" + toString(calculateVoltageDiff(circuitElement)) + "V");

                if (circuitElement.state != DrawState::WIRE) {
                    m_statusText.addText(", I= " + toString(circuitElement.current) + "A");
                }
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
                    nodes.push_back({ checkCircleCenter });

                    m_currentElement.addNode(nodes.back(), m_currentDrawState);

                    break;
                }

                hoverdCircle = checkCircleCenter;
                m_hoverTriggerd = true;
                break;
            }

        }
    }
}