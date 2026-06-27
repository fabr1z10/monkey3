#pragma once

#include <monkey3/math/walkarea_classes.h>
#include <monkey3/renderable.h>

class WalkArea : public Renderable {

public:

	/**
	 * Returns the area id of the area that contains the point P, or -1 if no area contains it
	 * @param P The point to check
	 * @return The area id, or -1 if no area contains it
	 */
	int findArea(glm::vec2 P) const;

	/**
	 * Finds a path from the start area to the goal area, if one exists.
	 * @param start The id of the starting area
	 * @param goal The id of the goal area
	 * @return A vector of area ids representing the path from start to goal, or an empty vector if no path exists
	 */
	std::vector<int> findAreaPath(int start, int goal) const;

	std::vector<glm::vec2> findPath(glm::vec2 start, glm::vec2 end) const;

	bool isBlocked(glm::vec2 A, glm::vec2 B) const;

	int addPortal(int a, int b, glm::vec2 P0, glm::vec2 P1);

	int addArea(const Polygon& poly);

	void setPortalEnabled(int id, bool enabled);

	int addObstacle(const Polygon& poly) ;

	void removeObstacle(int id) ;

	void render(Renderer&, glm::mat4 worldTransform) override;

private:

	std::vector<Area> _areas;
	std::vector<Portal> _portals;
	std::vector<Obstacle> _obstacles;
};