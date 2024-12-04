#pragma once
#include "UIConstants.h"
#include "NodeObject.h"
#include <list>

struct CircuitElementRenderInfo {
    Vector2 ElementVector{ 0, 0 };
    Vector2 origin{ 0, UI::cellSize / 2.0f };
    Rectangle destTextureRec{ 0.0f, 0.0f, UI::cellSize, UI::cellSize };
    float ElementLength{ 0 };
    float lineLength{ 0 };
    float rotation{ 0 };
    bool drawExtentions{ false };
    float opacity{ 1.0f };
};

class CircuitElement {
public:
    DrawState state{ DrawState::RESISTOR };
    NodeObject* startNode{ nullptr };
    NodeObject* endNode{ nullptr };
    float value{ 5.0f };
    float current{ 0.0f };

    void draw(Font font, TexturesArray& textures);
    void drawElement(Vector2 endPos, Font font, TexturesArray& textures);
    void drawGround(Vector2 endPos, Font font, TexturesArray& textures);
    void drawElementText(Font font, Vector2 endPos);
    void setOpacity(float value) { RenderInfo.opacity = value; }

    ~CircuitElement() {
        if (state == DrawState::GROUND && !endNode)
            delete endNode;
    }
protected:
    CircuitElementRenderInfo RenderInfo{};
};