#pragma once

#include <monkey3/shape.h>

namespace shapes {

	class Box : public Shape {
	public:
		Box(float width, float height, glm::vec2 anchor);

		bool contains(const glm::vec2& p) const override;

		void render(Renderer&, glm::mat4 worldTransform) override;

		RayCastHit raycastAxis(glm::vec3 origin, float length, Axis axis) const override;

	private:
		glm::vec2 _size;
		glm::vec2 _anchor;
	};

}