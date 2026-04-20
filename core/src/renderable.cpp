#include "monkey3/renderable.h"

QuadRenderable::QuadRenderable(int texture, glm::vec4 uvCoords, glm::vec2 size) : Renderable(),
    _uvCoords(uvCoords), _size(size), _texture(texture) {

}

void QuadRenderable::render(Renderer & r, glm::mat4 worldTransform) {

    glm::vec2 pos = glm::vec2(worldTransform[3]);
    glm::vec2 scaleX(worldTransform[0]);
    glm::vec2 scaleY(worldTransform[1]);

    glm::vec2 size = _size * glm::vec2(
        glm::length(scaleX),
        glm::length(scaleY)
    );

    r.submitQuad(pos, size, _uvCoords, _color, _texture);
}
