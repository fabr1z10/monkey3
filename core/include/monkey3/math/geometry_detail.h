#pragma once

#include <glm/glm.hpp>
#include <vector>

bool segmentIntersectsPolygonVec(const glm::vec2& A, const glm::vec2& B, const std::vector<glm::vec2>& poly);
