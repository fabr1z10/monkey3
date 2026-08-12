#pragma once

#include <string>
#include "monkey3/color.h"

std::string loadFile(const std::string& path);

bool parseHexColor(const std::string& str, Color& color);