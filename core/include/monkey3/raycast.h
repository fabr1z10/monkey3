#pragma once

#include <glm/glm.hpp>
class Collider;

enum Axis {
    X = 0,
    Y = 1,
    Z = 2
};

struct RayCastHit {
    RayCastHit();
    
    RayCastHit(bool collide, float l, glm::vec3 normal);
    
    void update(float length, Collider* collider, glm::vec3 normal, int segIndex);

    bool collide;

    float length;

    Collider* entity;

    glm::vec3 normal;

    int segmentIndex;
};