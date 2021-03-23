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

#include <map>
#include "astar.h"

std::vector<Node> Map::FindPath(const Node& star_node, Node& end_node, std::vector<Node>& graph)
{
	std::map<Node, Node> came_from;
	std::map<Node, float> cost_so_far;

	PriorityQueue<Node, float> frontier;
	frontier.put(star_node, 0);

	came_from[star_node] = star_node;
	cost_so_far[star_node] = 0;
	Node current;
	std::vector<Node> path;

	while (!frontier.empty())
	{
		current = frontier.get();

		if (current.position() == end_node.position())
		{
			break;
		}

		for (NodeIndex next : current.neighbors())
		{
			const float new_cost = cost_so_far[current]
				+ Distance(current.position(), graph[next].position());
			if (cost_so_far.find(graph[next]) == cost_so_far.end()
				|| new_cost < cost_so_far[graph[next]])
			{
				cost_so_far[graph[next]] = new_cost;
				const float priority = new_cost
					+ Distance(graph[next].position(), end_node.position());
				frontier.put(graph[next], priority);
				came_from[graph[next]] = current;
			}
		}
	}
	while (current.position() != star_node.position())
	{
		current = came_from[current];
		path.push_back(current);
	}
	std::reverse(path.begin(), path.end());
	return path;
}
