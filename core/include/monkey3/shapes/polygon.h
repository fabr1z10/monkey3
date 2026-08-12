#pragma once


#include <monkey3/shape.h>

namespace shapes {

	class Polygon : public Shape {
	public:
		Polygon(const std::vector<glm::vec2>& vertices);

		bool contains(const glm::vec2 &p) const override;

		void render(Renderer &, glm::mat4 worldTransform) override;

		bool segmentIntersects(const glm::vec2& a, const glm::vec2& b) const;
	private:
		std::vector<glm::vec2> _points;
	};

}