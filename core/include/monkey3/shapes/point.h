#pragma once


#include <monkey3/shape.h>

namespace shapes {

	class Point : public Shape {
	public:
		Point(float x, float y);

		bool contains(const glm::vec2 &p) const override;

		void render(Renderer &, glm::mat4 worldTransform) override;

	private:
		glm::vec2 _coords;
	};

}