#pragma once

#include <glm/glm.hpp>
#include <monkey3/color.h>
#include <monkey3/renderable.h>
#include <monkey3/bounds.h>
#include <monkey3/raycast.h>

namespace shapes {

	class Shape : public Renderable {
	public:
		virtual ~Shape() = default;

		/**
		 *  Tests whether a point is inside the shape in LOCAL space.
		 *  The input point MUST already be transformed into the shape's local coordinate system.
		 *  This function does not know anything about world transforms, nodes, or cameras.
		 *  Coordinate system:
		 *  - The shape is defined in its own local space
		 *  - (0,0) is the shape's origin
		 *  - Caller is responsible for converting world → local space
		 *
		 *   @param p Point in LOCAL space to test
		 *   @return true if the point is inside or on the boundary of the shape
		 */
		virtual bool contains(const glm::vec2 &p) const = 0;

		//virtual void render(Renderer &, glm::mat4 worldTransform) = 0;

		void setColor(Color);

		Bounds getBounds() const;

		virtual RayCastHit raycastAxis(glm::vec3 origin, float length, Axis axis) const = 0;
	protected:
		Color _color;
		Bounds _bounds;
	};

	inline void Shape::setColor(Color color) {
		_color = color;
	}

	inline Bounds Shape::getBounds() const {
		return _bounds;
	}

}