#pragma once

#include <string>
#include "monkey3/color.h"
#include <glm/glm.hpp>

const float epsilon = 0.0001f;


template <typename T>
int sign(T x)
{
    return x >= T{ 0 } ? 1 : -1;
}

template <class T>
bool isEqual(T x, T y, T eps = epsilon) {
    return std::abs(x - y) < eps;
}

template <class T>
bool isZero(T x, T eps = epsilon) {
    return isEqual(x, 0.0f, eps);
}


// returns the angle (in radians) between two vectors
float angle(glm::vec3 a, glm::vec3 b);

std::string loadFile(const std::string& path);

bool parseHexColor(const std::string& str, Color& color);