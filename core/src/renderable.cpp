#include "monkey3/renderable.h"
#include <monkey3/primitive.h>

QuadRenderable::QuadRenderable(int texture, glm::vec4 uvCoords, glm::vec2 size) : Renderable(),
    _uvCoords(uvCoords), _size(size), _texture(texture) {

}

void QuadRenderable::render(Renderer & r, glm::mat4 worldTransform) {

    glm::vec3 pos = worldTransform[3];
    glm::vec2 scaleX(worldTransform[0]);
    glm::vec2 scaleY(worldTransform[1]);

    glm::vec2 size = _size * glm::vec2(
        glm::length(scaleX),
        glm::length(scaleY)
    );
    
    QuadInfo info;
    info.position = pos;
	info.size = size;
	info.uvRect = _uvCoords;
    info.color = _color;
    info.textureId = _texture;
    r.submitGeometry(info);
}
