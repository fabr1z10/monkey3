#pragma once

#include <string>
#include "monkey3/color.h"

template <typename T>
int sign(T x)
{
    return x >= T{ 0 } ? 1 : -1;
}

std::string loadFile(const std::string& path);

bool parseHexColor(const std::string& str, Color& color);