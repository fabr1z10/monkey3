#include <monkey3/math/walkarea.h>
#include <monkey3/math/geometry.h>
#include <queue>
#include <unordered_map>
#include <algorithm>
#include <monkey3/render_helper.h>

int WalkArea::findArea(glm::vec2 P) const {
	for (const auto& area : _areas) {
		if (pointInPolygon(P, area.shape))
			return area.id;
	}
	return -1;
}

bool WalkArea::isBlocked(glm::vec2 A, glm::vec2 B) const {
	for (const auto& obs : _obstacles) {
		if (!obs.active) continue;

		if (segmentIntersectsPolygon(A, B, obs.shape))
			return true;
	}
	return false;
}

int WalkArea::addArea(const Polygon &poly) {
	int id = (int)_areas.size();
	_areas.push_back({id, poly, {}});
	return id;
}

int WalkArea::addPortal(int a, int b, glm::vec2 P0, glm::vec2 P1) {
	int id = (int)_portals.size();

	_portals.push_back({
							   id,
							   a,
							   b,
							   P0,
							   P1,
							   true});

	_areas[a].portals.push_back(id);
	_areas[b].portals.push_back(id);

	return id;
}

void WalkArea::setPortalEnabled(int id, bool enabled) {
	_portals[id].enabled = enabled;
}

int WalkArea::addObstacle(const Polygon& poly) {
	int id = (int)_obstacles.size();
	_obstacles.push_back({id, poly, true});
	return id;
}

void WalkArea::removeObstacle(int id) {
	_obstacles[id].active = false;
}


std::vector<int> WalkArea::findAreaPath(int start, int goal) const
{
	if (start == goal)
		return { start };

	std::queue<int> q;
	std::unordered_map<int, int> cameFrom;
	std::unordered_map<int, bool> visited;

	q.push(start);
	visited[start] = true;
	cameFrom[start] = -1;

	while (!q.empty())
	{
		int current = q.front();
		q.pop();

		const Area& area = _areas[current];

		for (int portalId : area.portals)
		{
			const Portal& p = _portals[portalId];
			if (!p.enabled)
				continue;

			int next = (p.a_area == current) ? p.b_area : p.a_area;

			if (visited[next])
				continue;

			visited[next] = true;
			cameFrom[next] = current;

			if (next == goal)
			{
				// reconstruct path
				std::vector<int> path;
				for (int at = goal; at != -1; at = cameFrom[at])
					path.push_back(at);

				std::reverse(path.begin(), path.end());
				return path;
			}

			q.push(next);
		}
	}

	return {}; // no path
}

std::vector<glm::vec2> WalkArea::findPath(glm::vec2 start, glm::vec2 end) const {
	std::vector<glm::vec2> result;
	int startArea = findArea(start);
	int endArea = findArea(end);
	if (startArea == -1 || endArea == -1) {
		return result; // no path
	}
	if (startArea == endArea) {
		if (!isBlocked(start, end)) {
			return {start, end};
		} else {
			return {};
		}
	}

	// 1. area path (graph BFS)
	auto areaPath = findAreaPath(startArea, endArea);
	if (areaPath.empty())
		return {};
	result.push_back(start);
	glm::vec2 current = start;
	for (size_t i = 0; i < areaPath.size()-1; i++) {
		int a = areaPath[i];
		int b = areaPath[i+1];
		// find portal connecting a and b
		const Portal* portal = nullptr;
		for (int portalId : _areas[a].portals) {
			const Portal &p = _portals[portalId];
			if (!p.enabled) continue;
			if ((p.a_area == a && p.b_area == b) || (p.a_area == b && p.b_area == a)) {
				portal = &p;
				break;
			}
		}
		if (!portal) {
			return {}; // no portal found, should not happen
		}
		glm::vec2 entry;
		glm::vec2 exit;
		if (portal->a_area == a) {
			entry = portal->a;
			exit = portal->b;
		} else {
			entry = portal->b;
			exit = portal->a;
		}
		if (isBlocked(current, entry)) {
			return {}; // blocked path to portal
		}
		result.push_back(entry);
		result.push_back(exit);
		current = exit;
	}
	// final segment to goal
	if (!isBlocked(current, end))
	{
		result.push_back(end);
		return result;
	}
	return {};
}

void WalkArea::render(Renderer & r, glm::mat4 worldTransform) {

	for (const auto& a : _areas) {
		renderPoly(r, a.shape.outer, true, glm::vec4(1.f), worldTransform);
		for (const auto& hole : a.shape.holes) {
			renderPoly(r, hole, true, glm::vec4(1.f), worldTransform);
		}
	}
}