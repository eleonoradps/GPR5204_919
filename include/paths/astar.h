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

#pragma once

#include <vector>
#include "maths/vector2.h"

namespace path {
	
	using NodeIndex = int;

// Class used to represent a node.
class Node {
public:
	Node() = default;
	Node(maths::Vector2f position, std::vector<NodeIndex> neighbors) {
		position_ = position;
		neighbors_ = neighbors;
	}

	// Return neighbors of a node.
	std::vector<NodeIndex> neighbors() const {
		return neighbors_;
	}

	// Return the position of a node.
	maths::Vector2f position() const {
		return position_;
	}

	bool operator<(const Node& other) const {
		return neighbors_ < other.neighbors_&& position_.x < other.position_.x
			&& position_.y < other.position_.y;
	}

	bool operator==(const Node& other) const {
		return neighbors_ == other.neighbors_ && position_.x == other.position_.x
			&& position_.y == other.position_.y;
	}

private:
	std::vector<NodeIndex> neighbors_;
	maths::Vector2f position_;
};

// Class used to represent a map.
class Map {
public:
	// Push a node in graph_.
	void AddNode(Node node) {
		graph_.push_back(node);
	}
	// Return graph_
	std::vector<Node> GetGraph() {
		return graph_;
	}
	// Calculate the distance between two vector2f.
	static float Distance(maths::Vector2f pos1, maths::Vector2f pos2) {
		return abs(sqrt((pow(pos2.x - pos1.x, 2)) + (pow(pos2.y - pos1.y, 2))));
	}
	// Find the lowest cost path with A* from the start node to the last node.
	std::vector<NodeIndex> FindPath(const NodeIndex& start_node, const NodeIndex& end_node);
private:
	std::vector<Node> graph_;
};

} // namespace path