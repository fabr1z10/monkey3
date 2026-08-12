#pragma once

#include <monkey3/component.h>
#include <monkey3/shape.h>

namespace components {

	class HotSpot : public Component {
	public:
		HotSpot(std::unique_ptr<shapes::Shape> shape);

		~HotSpot() override;

		void start() override;

		void update(double) override {}

		virtual void onMouseEnter() {}

		virtual void onMouseLeave() {}

		void render(Renderer &, glm::mat4 worldTransform) override;

	private:
		std::unique_ptr<shapes::Shape> _shape;
	};
}