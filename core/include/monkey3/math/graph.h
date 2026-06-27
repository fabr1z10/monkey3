#pragma once

#include <unordered_map>
#include <vector>


template<typename Node>
class Graph {
public:
	struct Edge {
		Node to;
		float cost;
		int portalId;
	};

	void add_node(const Node& node) {
		_adj[node];
	}

	void add_edge(const Node& from,
				  const Node& to,
				  float cost,
				  int portalId)
	{
		_adj[from].push_back({to, cost, portalId});
	}

	void add_edge_undirected(const Node& from,
						 const Node& to,
						 float cost,
						 int portalId)
	{
		add_edge(from, to, cost, portalId);
		add_edge(to, from, cost, portalId);
	}

	const std::vector<Edge>& neighbors(const Node& node) const {
		static const std::vector<Edge> empty;
		auto it = _adj.find(node);
		return (it != _adj.end()) ? it->second : empty;
	}

private:
	std::unordered_map<Node, std::vector<Edge>> _adj;
};

