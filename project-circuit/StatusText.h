#pragma once
#include "UIConstants.h"
#include <string>

class StatusText {
private:
    bool m_showStatus = false;
    std::string m_statusText{};
    Font m_font;

public:
    StatusText(Font font)
       : m_font(font)
    {}

    void addText(const std::string& text) { 
        m_showStatus = true;
        m_statusText += text;
    }

    void clearText() { m_statusText = ""; }

    void reset() { m_showStatus = false; }

    void draw() {
        if (m_showStatus) {
            DrawTextEx(m_font, m_statusText.c_str(), Vector2{ 20, 20 }, 20, 1, BLACK);
        }
    }
};