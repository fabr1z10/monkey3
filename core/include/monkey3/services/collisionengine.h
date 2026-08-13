#pragma once

#include <monkey3/service.h>
#include <monkey3/raycast.h>
#include <monkey3/hashpair.h>
#include <monkey3/components/collider.h>
#include <unordered_set>


struct CollisionEngineCell {
    bool dirty;
    std::unordered_set<Collider*> colliders;
};


class CollisionEngine : public IService {
public:
    virtual RayCastHit rayCastAxis(glm::vec3 origin, float length, int mask, Node* node, Axis axis) = 0;

protected:
    glm::vec3 _size;
    std::unordered_map<glm::ivec3, CollisionEngineCell> m_cells;

};

class CollisionEngine2D : public CollisionEngine {
public:
    RayCastHit rayCastAxis(glm::vec3 origin, float length, int mask, Node* node, Axis axis) override;
};