#include <monkey3/math/astar.h>


#include <queue>
#include <unordered_map>
#include <vector>
#include <limits>
#include <algorithm>



std::vector<int> astar(const Graph<int>& graph,
					   int start,
					   int goal)
{
	std::priority_queue<AStarNode, std::vector<AStarNode>, Compare> open;

	std::unordered_map<int, float> gScore;
	std::unordered_map<int, int> cameFrom;

	auto INF = std::numeric_limits<float>::infinity();

	gScore[start] = 0.0f;

	open.push({start, 0.0f});

	while (!open.empty())
	{
		int current = open.top().id;
		open.pop();

		if (current == goal)
			break;

		for (const auto& edge : graph.neighbors(current))
		{
			int next = edge.to;

			float tentativeG = gScore[current] + edge.cost;

			if (!gScore.count(next) || tentativeG < gScore[next])
			{
				cameFrom[next] = current;
				gScore[next] = tentativeG;

				float f = tentativeG + heuristic(next, goal);

				open.push({next, f});
			}
		}
	}

	// ---- reconstruct path ----
	std::vector<int> path;

	if (!cameFrom.count(goal) && start != goal)
		return {}; // no path

	int cur = goal;
	path.push_back(cur);

	while (cur != start)
	{
		cur = cameFrom[cur];
		path.push_back(cur);
	}

	std::reverse(path.begin(), path.end());
	return path;
}