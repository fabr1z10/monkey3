#pragma once

#include <glm/glm.hpp>
#include <monkey3/math/walkarea_classes.h>

/**
 * Computes the orientation of point C relative to the directed line A → B.
 *
 * Assumes a 2D Cartesian coordinate system with Y pointing UP.
 *
 * Mathematically, this returns the signed area of the triangle (A, B, C)
 * multiplied by 2.
 *
 * @param A First point of the segment
 * @param B Second point of the segment
 * @param C Test point
 *
 * @return
 *   > 0 : C is to the left of A→B
 *   < 0 : C is to the right of A→B
 *   = 0 : A, B, C are collinear (within floating-point precision)
 */
float orient(const glm::vec2& a, const glm::vec2& b, const glm::vec2& c);

/**
 * Checks whether P is in the axis-aligned bounding box defined by A and B.
 * @param a First point
 * @param b Second point
 * @param p Test point
 * @return True if P is inside the bounding box, false otherwise
 */
bool isInAABB(const glm::vec2& a, const glm::vec2& b, const glm::vec2& p);

bool segmentsIntersect(const glm::vec2& p1, const glm::vec2& p2, const glm::vec2& q1, const glm::vec2& q2);

bool segmentIntersectsLoop(const glm::vec2& p1, const glm::vec2& p2, const std::vector<glm::vec2>& loop);

bool pointInPolygon(const glm::vec2&, const Polygon&);


bool segmentIntersectsPolygon(glm::vec2 A, glm::vec2 B, const Polygon& poly);

/**
 * Tells whether P lies on segment AB
 * @param a First segment point
 * @param b Second segment point
 * @param p The test point
 * @return True if P lies on segment AB, false otherwise
 */
bool pointOnSegment(const glm::vec2& a, const glm::vec2& b, const glm::vec2& p);

bool pointInSimplePolygon(const glm::vec2& p, const std::vector<glm::vec2>& poly);

