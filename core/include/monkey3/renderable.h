#pragma once

#include "renderer.h"

class Renderable {
public:
    virtual void render(Renderer&, glm::mat4 worldTransform) = 0;
};


class QuadRenderable : public Renderable {

public:
    QuadRenderable(int texture, glm::vec4 uvCoords, glm::vec2 size);
    void render(Renderer&, glm::mat4 worldTransform) override;
private:
    int _texture;
    glm::vec4 _uvCoords;
    glm::vec2 _size;
    glm::vec4 _color = glm::vec4(1.f, 1.f, 1.f, 1.f);


};