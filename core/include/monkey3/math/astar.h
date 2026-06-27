#include <monkey3/math/graph.h>
#include <vector>


float heuristic(int a, int b)
{
	// TEMP: zero heuristic = Dijkstra (safe start)
	return 0.0f;
}

struct AStarNode {
	int id;
	float f;
};

struct Compare {
	bool operator()(const AStarNode& a, const AStarNode& b) {
		return a.f > b.f; // min-heap
	}
};

std::vector<int> astar(const Graph<int>& graph, int start, int goal);
