#pragma once

#include <monkey3/component.h>
#include <monkey3/shape.h>
#include <monkey3/bounds.h>

class Collider : public Component {
public:
	Collider(int flag, int mask, int tag);
	
	void start() override;

	void update(float) override {}

	int getCollisionFlag() const;

	Bounds getStaticBounds();
	
	virtual const shapes::Shape* getShape() const = 0;
private:
	int _flag;
	int _mask;
	int _tag;
};


class SimpleCollider : public Collider {
public:
	SimpleCollider(std::unique_ptr<shapes::Shape>, int flag, int mask, int tag);

	const shapes::Shape* getShape() const override;

	void render(Renderer&, glm::mat4 worldTransform) override;

private:
	std::unique_ptr<shapes::Shape> _shape;
};

