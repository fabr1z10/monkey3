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

struct ColliderInfo {
	glm::ivec3 min;
	glm::ivec3 max;
	bool dirty;
};


class CollisionEngine : public IService {
public:
    /**
     * @brief Constructs a collision engine using the specified cell size.
     *
     * @param size The dimensions of each collision grid cell along the X, Y,
     *             and Z axes.
     */
    CollisionEngine(glm::vec3 size);

    virtual RayCastHit rayCastAxis(glm::vec3 origin, float length, int mask, Node* node, Axis axis) = 0;

	void add (Collider*);

	void move (Collider*);

	void remove (Collider*) ;

	int getIndex(float u, int d);


protected:
	virtual void pushCollider(Collider* c, glm::ivec3 m, glm::ivec3 M) = 0;

	std::pair<glm::ivec3, glm::ivec3> getLocation(const Bounds& b);

    glm::vec3 _size;

	glm::vec3 _invSize;
    std::unordered_map<glm::ivec3, CollisionEngineCell> _cells;
	std::unordered_map<Collider*, ColliderInfo> _colliderLocations;
};

inline int CollisionEngine::getIndex(float u, int d)
{
	return _invSize[d] == 0.f
		   ? 0
		   : static_cast<int>(std::floor(u * _invSize[d]));
}

class CollisionEngine2D : public CollisionEngine {
public:
    using CollisionEngine::CollisionEngine;

    RayCastHit rayCastAxis(glm::vec3 origin, float length, int mask, Node* node, Axis axis) override;
protected:
	void pushCollider(Collider* c, glm::ivec3 m, glm::ivec3 M) override;

};