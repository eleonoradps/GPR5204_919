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
#include "paths/astar.h"
#include "paths/inverted_priority_queue.h"

namespace path {

std::vector<NodeIndex> Map::FindPath(NodeIndex start_node, NodeIndex end_node) {
	
	// This queue contains next nodes where we will check these neighbors.
	PriorityQueue<NodeIndex, float> frontier;
	frontier.put(start_node, 0);

	came_from_[start_node] = start_node;
	cost_so_far_[start_node] = 0;
	NodeIndex current;

	while (!frontier.empty()) {
		// Get the lowest priority node.
		current = frontier.get();

		if (current == end_node) {
			break;
		}

		for (NodeIndex next : graph_[current].neighbors()) {
			// The cost to get to the current node added to the distance to the neighbor.
			const float new_cost = cost_so_far_[current]
				+ maths::Vector2f{ graph_[current].position().x
					- graph_[next].position().x
					, graph_[current].position().y
					- graph_[next].position().y }.Magnitude(); 
			/* Check if the node has been checked and if cost to go to the next
			node from current is less than the lowest cost saved to go to the
			next node.*/
			if (cost_so_far_.find(next) == cost_so_far_.end() 
				|| new_cost < cost_so_far_[next]) {
				// Put the new lowest cost to go to next node.
				cost_so_far_[next] = new_cost; 
				// Calculate the heuristic.
				const float priority = new_cost
					+ maths::Vector2f{ graph_[next].position().x
						- graph_[end_node].position().x
						, graph_[next].position().y
						- graph_[end_node].position().y }.Magnitude();
				// Add to nodes where we will check these neighbors.
				frontier.put(next, priority); 
				/* Save the current node with the lowest cost to go to the next
				node. */
				came_from_[next] = current; 
			}
		}
	}
	/* Return an empty vector of NodeIndex if there is no path to go to the end
	node.*/
	if (current != end_node) {
		std::vector<NodeIndex> no_path_vector;
		return no_path_vector;
	}
	// Add NodeIndex of nodes with the lowest cost to go to each node.
	path_.push_back(current);
	while (current != start_node) {
		current = came_from_[current];
		path_.push_back(current);
	}
	// Reverse path to start with the start node.
	return { path_.rbegin(), path_.rend() };
}

}  // namespace path
