/*
MIT License

Copyright (c) 2021 SAE Institute Geneva

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <unordered_map>
#include "astar.h"

std::vector<NodeIndex> Map::FindPath(const NodeIndex& start_node, const NodeIndex& end_node)
{
	// The node with the lowest cost to go to the key node.
	std::unordered_map<NodeIndex, NodeIndex> came_from;
	// The lowest cost to go to a node.
	std::unordered_map<NodeIndex, float> cost_so_far;

	// Next nodes where we will check these neighbors.
	PriorityQueue<NodeIndex, float> frontier;
	frontier.put(start_node, 0);

	came_from[start_node] = start_node;
	cost_so_far[start_node] = 0;
	NodeIndex current;
	std::vector<NodeIndex> path;

	while (!frontier.empty()) {
		current = frontier.get(); // Get the lowest priority node.

		if (current == end_node) {
			break;
		}

		for (NodeIndex next : graph_[current].neighbors()) {
			const float new_cost = cost_so_far[current]
				+ Distance(graph_[current].position(), graph_[next].position()); // Cost to go to the current node + distance to go to the neighbor.
			if (cost_so_far.find(next) == cost_so_far.end() // Check if the node has been checked.
				|| new_cost < cost_so_far[next]) { // Check if cost to go to the next node from current is less than the lowest cost saved to go to the next node.
				cost_so_far[next] = new_cost; // Put the new lowest cost to go to next node.
				const float priority = new_cost 
					+ Distance(graph_[next].position()
					, graph_[end_node].position()); // Calculate the heuristic.
				frontier.put(next, priority); // Add to nodes where we will check these neighbors.
				came_from[next] = current; // Save the current node with the lowest cost to go to the next node.
			}
		}
	}
	// Add NodeIndex of nodes with the lowest cost to go to each node.
	path.push_back(current);
	while (current != start_node){
		current = came_from[current];
		path.push_back(current);
	}
	// Reverse path to start with the start node.
	std::reverse(path.begin(), path.end());
	return path;
}
