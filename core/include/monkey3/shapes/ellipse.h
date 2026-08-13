#pragma once

#include <monkey3/shape.h>

namespace shapes {

	class Ellipse : public Shape {
	public:
		Ellipse(float rx, float ry);

		bool contains(const glm::vec2 &p) const override;

		void render(Renderer &, glm::mat4 worldTransform) override;

		RayCastHit raycastAxis(glm::vec3 origin, float length, Axis axis) const override;

	private:
		glm::vec2 _radii;
		int _segments;
	};

}