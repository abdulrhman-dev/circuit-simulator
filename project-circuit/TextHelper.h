#ifndef TEXT_HELPER_H
#define TEXT_HELPER_H
#include <map>
#include <utility>
#include <sstream>
#include <optional>
#include "UIConstants.h"

using MetricPrefix = std::pair<char, float>;

inline std::map<char, float> metricPrefixes{
	{'k', 1e3f},
	{'m', 1e-3f},
	{'G', 1e9f}, 
	{'M', 1e6f}, 
	{'u', 1e-6f},
	{'n', 1e-9f},
	{'p', 1e-12f},
};

std::string toString(float value);
std::string getUnit(DrawState drawState);
std::string getDisplayText(float value, std::optional<DrawState> state = std::nullopt);

#endif

