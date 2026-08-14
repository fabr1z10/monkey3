#include <monkey3/bounds.h>

Bounds::Bounds() : min(glm::vec3(0.0f)), max(glm::vec3(0.0f)) {

}

Bounds::Bounds(glm::vec3 P1, glm::vec3 P2) {
    min = P1; 
    max = P1;
    addPoint(P2);
}

bool Bounds::isVoid() const {
    return (max.x == min.x && max.y == min.y && max.z == min.z);
}

void Bounds::translate(glm::vec3 delta) {
    min += delta;
    max += delta;
}

void Bounds::addPoint(glm::vec3 P) {
    min.x = std::min(min.x, P.x);
    max.x = std::max(max.x, P.x);
    min.y = std::min(min.y, P.y);
    max.y = std::max(max.y, P.y);
    min.z = std::min(min.z, P.z);
    max.z = std::max(max.z, P.z);
}


glm::vec3 Bounds::getCenter() {
    return 0.5f * (min + max);
}