#include "Output.h"

void CurrentCircuitElement::update(DrawState currState) {
    if (!drawingElement || !startNode)
        return;

    state = currState;

    reCalculateRenderInfo(GetMousePosition());
}

void CurrentCircuitElement::reCalculateRenderInfo(Vector2 endPos) {
    RenderInfo.destTextureRec.x = startNode->pos.x;
    RenderInfo.destTextureRec.y = startNode->pos.y;

    RenderInfo.ElementVector = Vector2Subtract(endPos, Vector2{ startNode->pos.x, startNode->pos.y });
    RenderInfo.ElementLength = Vector2Length(RenderInfo.ElementVector);

    RenderInfo.lineLength = (Vector2Length(RenderInfo.ElementVector) - RenderInfo.destTextureRec.width) / 2.0f;

    RenderInfo.rotation = 180.0f - (Vector2Angle(Vector2Subtract(Vector2{ startNode->pos.x, startNode->pos.y }, endPos), Vector2{ 1,0 }) * 180.0f / PI);

    if (RenderInfo.ElementLength > UI::cellSize) {
        RenderInfo.destTextureRec.width = UI::cellSize;
        RenderInfo.destTextureRec.height = UI::cellSize;
        RenderInfo.origin = { 0,  RenderInfo.destTextureRec.height / 2.0f };

        RenderInfo.drawExtentions = true;
        RenderInfo.origin.x = -RenderInfo.lineLength;
    }
    else {
        RenderInfo.destTextureRec.width = RenderInfo.ElementLength;
        RenderInfo.destTextureRec.height = RenderInfo.ElementLength;

        RenderInfo.drawExtentions = false;
        RenderInfo.origin = { 0,  RenderInfo.destTextureRec.height / 2.0f };
    }
}

void CurrentCircuitElement::addNode(NodeObject& node, DrawState currDrawState) {
    if (drawingElement) {
        endNode = &node;
        state = currDrawState;
        endNode->addEdge(startNode, currDrawState);
        startNode->addEdge(endNode, currDrawState);
        reCalculateRenderInfo(endNode->pos);
        m_circuitElements.push_back(*this);
        reset();
    }
    else {
        startNode = &node;
    }

    drawingElement = !drawingElement;
}

void CurrentCircuitElement::reset() {
    if (drawingElement)
        return;

    startNode = nullptr;
    endNode = nullptr;
    value = 5.0f;
    current = 0.0f;
}

void CurrentCircuitElement::draw(Font font, TexturesArray& textures) {
    if (!drawingElement)
        return;

    if (state == DrawState::WIRE) {
        DrawLineEx(startNode->pos, GetMousePosition(), 2.0f, UI::WIRE_COLOR);
        return;
    }

    drawElement(GetMousePosition(), font, textures);
}