#include "CircuitElement.h"
#include "TextHelper.h"
#include "raymath.h"

void CircuitElement::draw(Font font, TexturesArray& textures) {
    if (state == DrawState::WIRE) {
        DrawLineEx(startNode->pos, endNode->pos, 2.0f, Fade(UI::WIRE_COLOR, RenderInfo.opacity));
        RenderInfo.opacity = 1.0f;
        return;
    }
    else if (state == DrawState::GROUND) {
        drawGround({0, 0}, font, textures);
        RenderInfo.opacity = 1.0f;
        return;
    }

    if(endNode)
        drawElement(endNode->pos, font, textures);
}

void CircuitElement::drawElement(Vector2 endPos, Font font, TexturesArray& textures) {
    Texture2D currentTexture = textures[static_cast<std::size_t>(state)];
    Rectangle sourceTexture = { 0.0f, 0.0f, (float)currentTexture.width, (float)currentTexture.height };
      
    drawElementText(font, endPos);

    DrawTexturePro(currentTexture, sourceTexture, RenderInfo.destTextureRec, RenderInfo.origin, RenderInfo.rotation, Fade(WHITE, RenderInfo.opacity));

    if (RenderInfo.drawExtentions) {
        Vector2 startPos = { RenderInfo.destTextureRec.x, RenderInfo.destTextureRec.y };
        Vector2 lineVector = Vector2Scale(Vector2Normalize(RenderInfo.ElementVector), RenderInfo.lineLength);

        DrawLineEx(startPos, Vector2Add(startPos, lineVector), 2.0f, Fade(UI::WIRE_COLOR, RenderInfo.opacity));
        DrawLineEx(Vector2Subtract(endPos, lineVector), endPos, 2.0f, Fade(UI::WIRE_COLOR, RenderInfo.opacity));
    }

    RenderInfo.opacity = 1.0f;
}

void CircuitElement::drawGround(Vector2 endPos, Font font, TexturesArray& textures) {
    Texture2D currentTexture = textures[static_cast<std::size_t>(state)];
    Rectangle sourceTexture = { 0.0f, 0.0f, (float)currentTexture.width, (float)currentTexture.height };

    DrawTexturePro(currentTexture, sourceTexture, RenderInfo.destTextureRec, RenderInfo.origin, RenderInfo.rotation, Fade(WHITE, RenderInfo.opacity));

    if (RenderInfo.drawExtentions) {
        Vector2 startPos = { RenderInfo.destTextureRec.x, RenderInfo.destTextureRec.y };
        Vector2 lineVector = Vector2Scale(Vector2Normalize(RenderInfo.ElementVector), RenderInfo.lineLength);

        DrawLineEx(startPos, Vector2Add(startPos, lineVector), 2.0f, Fade(UI::WIRE_COLOR, RenderInfo.opacity));
    }

    RenderInfo.opacity = 1.0f;
}

void CircuitElement::drawElementText(Font font, Vector2 endPos ) {
    if (RenderInfo.ElementLength < UI::cellSize / 2.0f)
        return;

    std::string text{ getDisplayText(value, state)};

    bool isInLeftSide = (RenderInfo.rotation >= 0 && RenderInfo.rotation <= 90) || (RenderInfo.rotation <= 360 && RenderInfo.rotation >= 270);

    float fontSize = 13;
    Vector2 textSize = MeasureTextEx(font, text.c_str(), fontSize, 1.0f);

    float textWidth = textSize.x;
    float textHeight = textSize.y;

    float extraDistance = abs(textWidth / 2.4f * sin(DEG2RAD * (RenderInfo.rotation)));
    float distance = state == DrawState::RESISTOR ? 14.0f + extraDistance : 25.0f + extraDistance;

    Vector2 PerpendicularUnitVector = Vector2Normalize(Vector2{ (!isInLeftSide ? -1.0f : 1.0f) * RenderInfo.ElementVector.y, (isInLeftSide ? -1.0f : 1.0f) * RenderInfo.ElementVector.x });
    Vector2 perpendicularElementVector = Vector2Scale(PerpendicularUnitVector, distance);
    Vector2 midpoint = Vector2Subtract(endPos, Vector2Scale(RenderInfo.ElementVector, 0.5f));

    Vector2 textPos = Vector2Add(midpoint, perpendicularElementVector);

    textPos.x -= textWidth / 2.0f;
    textPos.y -= textHeight / 2.0f;


    DrawTextEx(font, text.c_str(), textPos, fontSize, 1, BLACK);
}