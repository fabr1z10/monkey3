#include <monkey3/services/collisionengine.h>

CollisionEngine::CollisionEngine(glm::vec3 size) : IService(), _size(size) {

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

    int cellStart =
        (p0 >= 0.0f ? 0 : -1) +
        static_cast<int>(p0 / _size[axis]);

    int cellEnd =
        (p1 >= 0.0f ? 0 : -1) +
        static_cast<int>(p1 / _size[axis]);

    int cells = abs(cellEnd - cellStart) + 1;
    int inc = (p1 > p0) ? 1 : -1;

    glm::ivec3 cell;
    cell[axis] = cellStart;

    for (int a = 0; a < 3; ++a) {
        if (a == axis) {
            cell[a] = cellStart;
        } else {
            cell[a] = (origin[a] >= 0.0f ? 0 : -1) + static_cast<int>(origin[a] / _size[a]);
        }
    }

    //ShapeType boxType =
    //    _use3D ? ShapeType::AABB3D : ShapeType::AABB2D;

    for (int n = 0, i = cellStart;
        n < cells;
        ++n, i += inc)
    {
        cell[axis] = i;

        auto it = m_cells.find(cell);
        if (it == m_cells.end()) {
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

            auto result = shape->raycastAxis(origin, length, axis);

            if (!out.collide || (out.collide && result.length < out.length)) {
                out = result;
            }
        }

    }

    return out;
}