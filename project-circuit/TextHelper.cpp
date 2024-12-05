#include <sstream>
#include <math.h>

#include "TextHelper.h"
#include "UIConstants.h"

std::string toString(float value) {
    std::stringstream ss;
    ss << value;
    return ss.str();
}

std::string getUnit(DrawState drawState) {
    switch (drawState)
    {
    case DrawState::RESISTOR:
        return "\xCE\xA9";
    case DrawState::VOLTAGE_SOURCE:
        return "V";
    case DrawState::CURRENT_SOURCE:
        return "A";
    default:
        return "";
    }
}

std::optional<std::pair<char, float>> getPrefix(float value) {
    for (auto prefix : metricPrefixes) {
        if (prefix.second * 1 <= std::abs(value) && prefix.second * 1000 > std::abs(value))
            return prefix;
    }

    return {};
}

std::string getDisplayText(float value, std::optional<DrawState> state) {
    std::string text;
    auto chosenPrefix{getPrefix(value)};

    if (chosenPrefix.has_value()) {
        value = value/ chosenPrefix.value().second;
        if (chosenPrefix.value().first == 'u')
            text += "\xCE\xBC";
        else
            text += chosenPrefix.value().first;
    }
    text.insert(0, toString(std::round((value * 100.0f)) / 100.0f));

    if(state.has_value())
        text += getUnit(state.value());

    return text;
}