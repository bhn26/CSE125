#pragma once

#include <string>

#define ZERO_MEM(a) memset(a, 0, sizeof(a))
#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof((a)[0]))

#define INVALID_UNIFORM_LOCATION 0xffffffff

namespace Utils
{
    bool ReadFile(const std::string& filename, std::string& outString);
    double CurrentTime(); // In seconds
}
