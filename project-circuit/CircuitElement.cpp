#include "Output.h"

void CircuitElement::draw(Font font, TexturesArray& textures) {
    if (state == DrawState::WIRE) {
        DrawLineEx(startNode->pos, endNode->pos, 2.0f, UI::WIRE_COLOR);
        return;
    }

    drawElement(endNode->pos, font, textures);
}

void CircuitElement::drawElement(Vector2 endPos, Font font, TexturesArray& textures) {
    Texture2D currentTexture = textures[static_cast<std::size_t>(state)];
    Rectangle sourceTexture = { 0.0f, 0.0f, (float)currentTexture.width, (float)currentTexture.height };

      
    drawElementText(font, endPos);

    DrawTexturePro(currentTexture, sourceTexture, RenderInfo.destTextureRec, RenderInfo.origin, RenderInfo.rotation, WHITE);

    if (RenderInfo.drawExtentions) {
        DrawLineEx({ RenderInfo.destTextureRec.x, RenderInfo.destTextureRec.y }, Vector2Add({ RenderInfo.destTextureRec.x, RenderInfo.destTextureRec.y }, Vector2Scale(Vector2Normalize(RenderInfo.ElementVector), RenderInfo.lineLength)), 2.0f, UI::WIRE_COLOR);
        DrawLineEx(Vector2Subtract(endPos, Vector2Scale(Vector2Normalize(RenderInfo.ElementVector), RenderInfo.lineLength)), endPos, 2.0f, UI::WIRE_COLOR);
    }
}

void CircuitElement::drawElementText(Font font, Vector2 endPos ) {
    if (RenderInfo.ElementLength < UI::cellSize / 2.0f)
        return;

    std::string text{ toString(value) + getUnit(state) };

    bool isInLefSide = (RenderInfo.rotation >= 0 && RenderInfo.rotation <= 90) || (RenderInfo.rotation <= 360 && RenderInfo.rotation >= 270);

    int textHeight = 13;
    int textWidth = MeasureText(text.c_str(), textHeight);

    float extraDistance = abs(textWidth / 3.0f * sin(DEG2RAD * (RenderInfo.rotation)));
    float distance = state == DrawState::RESISTOR ? 14.0f + extraDistance : 25.0f + extraDistance;

    Vector2 PerpendicularUnitVector = Vector2Normalize(Vector2{ (!isInLefSide ? -1.0f : 1.0f) * RenderInfo.ElementVector.y, (isInLefSide ? -1.0f : 1.0f) * RenderInfo.ElementVector.x });
    Vector2 perpendicularElementVector = Vector2Scale(PerpendicularUnitVector, distance);
    Vector2 midpoint = Vector2Subtract(endPos, Vector2Scale(RenderInfo.ElementVector, 0.5f));

    Vector2 textPos = Vector2Add(midpoint, perpendicularElementVector);

    textPos.x -= textWidth / 2.0f;
    textPos.y -= textHeight / 2.0f;


    DrawTextEx(font, text.c_str(), textPos, textHeight, 1, BLACK);
}