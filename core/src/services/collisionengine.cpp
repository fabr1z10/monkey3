#include <monkey3/services/collisionengine.h>
#include <monkey3/node.h>

CollisionEngine::CollisionEngine(glm::vec3 size) : IService(), _size(size) {
	for (int i = 0; i < 3; ++i) {
		_invSize[i] = (_size[i] == 0.f) ? 0.f : (1.f / _size[i]);
	}
}

RayCastHit CollisionEngine2D::rayCastAxis(
    glm::vec3 origin,
    float length,
    int mask,
    Node* node,
    Axis axis)
{
    RayCastHit out;

    if (axis == Axis::Z) {
        return out;
    }

    float p0 = origin[axis];
    float p1 = p0 + length;

    int cellStart = getIndex(p0, axis);
    int cellEnd = getIndex(p1, axis);

    int cells = abs(cellEnd - cellStart) + 1;
    int inc = (p1 > p0) ? 1 : -1;

    glm::ivec3 cell(0);
    cell[axis] = cellStart;

	int other = 1 - axis;
	cell[other] = getIndex(origin[other], other);

    for (int n = 0, i = cellStart;
        n < cells;
        ++n, i += inc)
    {
        cell[axis] = i;

        auto it = _cells.find(cell);
        if (it == _cells.end()) {
            continue;
        }

        float segStart =
            (n == 0)
            ? p0
            : (i + (inc == 1 ? 0 : -1)) * _size[axis];

        float segEnd =
            (n == cells - 1)
            ? p1
            : (i + (inc == 1 ? 1 : 0)) * _size[axis];

        float segMin = segStart;
        float segMax = segEnd;

        if (inc == -1) {
            std::swap(segMin, segMax);
        }

        glm::vec3 min = origin;
        glm::vec3 max = origin;

        min[axis] = segMin;
        max[axis] = segMax;

        Bounds segmentBounds(min, max);

        for (const auto& c : it->second.colliders) {

            if (!c->isActive() || node == c->getNode())
            {
                continue;
            }

            int flag = c->getCollisionFlag();

            if (mask != 0 && (flag & mask) == 0) {
                continue;
            }

            auto shapeBounds = c->getStaticBounds();

            //if (!aabbTest(segmentBounds, shapeBounds)) {
            //    continue;
            //}

            auto* shape = c->getShape();
			auto colliderPosition = c->getNode()->getWorldPosition();
            auto result = shape->raycastAxis(origin - colliderPosition, length, axis);

            if (!out.collide || (out.collide && result.collide && result.length < out.length)) {
                out = result;
            }
        }

    }

    return out;
}

void CollisionEngine::add(Collider * c) {
	// this is called when a new collider starts. It registers with the engine
	// get the shape bounding box, transform it, map it
	auto aabb = c->getStaticBounds();
	if (!aabb.isVoid()) {
		auto loc = getLocation(aabb);
		pushCollider(c, loc.first, loc.second);
	}
}

void CollisionEngine::move(Collider * c) {
	add(c);
}

void CollisionEngine::remove(Collider * c) {
	//m_removed.insert(c);
	if (auto it = _colliderLocations.find(c); it != _colliderLocations.end()) {
		auto d = it->second;
		for (auto i = d.min.x; i <= d.max.x; ++i) {
			for (auto j = d.min.y; j <= d.max.y; ++j) {
				for (auto k = d.min.z; k <= d.max.z; ++k) {
					auto aa = _cells.find(glm::vec3(i, j, k));
					if (aa != _cells.end()) {
						aa->second.colliders.erase(c);
					}
				}
			}
		}
		_colliderLocations.erase(c);
	}

}


std::pair<glm::ivec3, glm::ivec3> CollisionEngine::getLocation(const Bounds &b) {
	glm::ivec3 min(0);
	glm::ivec3 max(0);
	min.x = getIndex(b.min.x, 0);
	min.y = getIndex(b.min.y, 1);
	min.z = getIndex(b.min.z, 2);
	max.x = getIndex(b.max.x, 0);
	max.y = getIndex(b.max.y, 1);
	max.z = getIndex(b.max.z, 2);
	return std::make_pair(min, max);
}

void CollisionEngine2D::pushCollider(Collider* c, glm::ivec3 m, glm::ivec3 M) {
	auto it = _colliderLocations.find(c);
	if (it != _colliderLocations.end()) {
		if (it->second.min != m || it->second.max != M) {
			for (int i = it->second.min.x; i <= it->second.max.x; ++i) {
				for (int j = it->second.min.y; j <= it->second.max.y; ++j) {
					_cells[glm::ivec3(i, j, 0)].colliders.erase(c);
				}
			}
		}
	}
	for (int i = m.x; i <= M.x; ++i) {
		for (int j = m.y; j <= M.y; ++j) {
			auto &cell = _cells[glm::ivec3(i, j, 0)];
			cell.colliders.insert(c);
			cell.dirty = true;
		}
	}
	_colliderLocations[c] = ColliderInfo {m, M, true};
}