#ifndef TEXT_HELPER_H
#define TEXT_HELPER_H
#include "Output.h"
#include <sstream>
namespace {
    inline std::string toString(float value) {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }

    inline std::string getUnit(DrawState drawState) {
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
}
#endif

