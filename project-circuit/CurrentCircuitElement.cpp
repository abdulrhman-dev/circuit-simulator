#include "CurrentCircuitElement.h"
#include "raymath.h"
#include "iostream"

void CurrentCircuitElement::update(Camera2D& camera, DrawState currState, std::list<NodeObject>& nodes) {
    if (!drawingElement || !startNode)
        return;

    bool groundWithNewStartNode = currState == DrawState::GROUND && startNode->graphNode.edges.size() == 0;

    if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT) || IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_Z) || groundWithNewStartNode) {
        reset();

        if (nodes.back().graphNode.edges.size() == 0)
            nodes.pop_back();

        return;
    }

    state = currState;

    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);

    reCalculateRenderInfo(mousePos);
}

void CurrentCircuitElement::reCalculateRenderInfo(Vector2 endPos) {
    RenderInfo.destTextureRec.x = startNode->pos.x;
    RenderInfo.destTextureRec.y = startNode->pos.y;

    RenderInfo.ElementVector = Vector2Subtract(endPos, Vector2{ startNode->pos.x, startNode->pos.y });
    RenderInfo.ElementLength = Vector2Length(RenderInfo.ElementVector);

    if(state != DrawState::GROUND)
        RenderInfo.lineLength = (RenderInfo.ElementLength - RenderInfo.destTextureRec.width) / 2.0f;
    else
        RenderInfo.lineLength = (RenderInfo.ElementLength - RenderInfo.destTextureRec.width);


    RenderInfo.rotation = -(Vector2Angle(RenderInfo.ElementVector, Vector2{ 1,0 }) * RAD2DEG);

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

       if (startNode->graphNode.id == endNode->graphNode.id)
            return;

        if (startNode->isNeighbor(*endNode))
            return;

        state = currDrawState;
        endNode->addEdge(startNode, currDrawState);
        startNode->addEdge(endNode, currDrawState);
        reCalculateRenderInfo(endNode->pos);
        m_circuitElements.push_back(*this);

        reset();
        return;
    }
    
    startNode = &node;
    drawingElement = !drawingElement;
}

void CurrentCircuitElement::addNode(Vector2 pos, DrawState currDrawState) {
    if (currDrawState != DrawState::GROUND)
        return;

    if (drawingElement) {
        endNode = new NodeObject(pos);
        startNode->graphNode.isGround = true;
        reCalculateRenderInfo(endNode->pos);
        m_circuitElements.push_back(*this);

        reset();
        return;
    }
}

void CurrentCircuitElement::reset() {
    startNode = nullptr;
    endNode = nullptr;
    value = 5.0f;
    current = 0.0f;
    drawingElement = false;
}

void CurrentCircuitElement::draw(Camera2D& camera, Font font, TexturesArray& textures) {
    if (!drawingElement || !startNode)
        return;

    Vector2 mousePos = GetScreenToWorld2D(GetMousePosition(), camera);

    if (state == DrawState::WIRE) {
        DrawLineEx(startNode->pos, mousePos, 2.0f, UI::WIRE_COLOR);
        return;
    }

    if(state == DrawState::GROUND) {
        drawGround(mousePos, font, textures);
        return;
    }

    drawElement(mousePos, font, textures);
}