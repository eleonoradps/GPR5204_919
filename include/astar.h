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

#include <queue>
#include <vector>
#include <functional>
#include "maths/vector2.h"

using NodeIndex = int;

class Node {
public:
	Node() = default;
	Node(maths::Vector2f position, std::vector<NodeIndex> neighbors)
	{
		position_ = position;
		neighbors_ = neighbors;
	}
	
	std::vector<NodeIndex> neighbors() const {
		return neighbors_;
	}

	maths::Vector2f position() const {
		return position_;
	}

	bool operator<(const Node& other) const {
		return neighbors_ < other.neighbors_ && position_.x < other.position_.x
		&& position_.y < other.position_.y;
	}

	bool operator==(const Node& other) const {
		return neighbors_ == other.neighbors_&& position_.x == other.position_.x
			&& position_.y == other.position_.y;
	}

private:
	std::vector<NodeIndex> neighbors_;
	maths::Vector2f position_;
};

class Map {
public:
	void AddNode(Node node){
		graph_.push_back(node);
	}
	std::vector<Node> GetGraph(){
		return graph_;
	}
	static float Distance(maths::Vector2f pos1, maths::Vector2f pos2) {
		return abs(sqrt((pow(pos2.x - pos1.x, 2)) + (pow(pos2.y - pos1.y, 2))));
	}
	// Find the lowest cost path with A* from the start node to the last node.
	std::vector<NodeIndex> FindPath(const NodeIndex& start_node, const NodeIndex& end_node);
private:
	std::vector<Node> graph_;
};

// Queue who sort the element from lowest to higher.
template<typename T, typename priority_t>
class PriorityQueue {
public:
typedef std::pair<priority_t, T> PQElement;
std::priority_queue<PQElement, std::vector<PQElement>,
std::greater<PQElement>> elements;

// Return if the queue is empty.
bool empty() const {
	return elements.empty();
}

// Put elements in the queue.
void put(T item, priority_t priority) {
	elements.emplace(priority, item);
}

// Return the element with the lowest priority.
T get() {
	T best_item = elements.top().second;
	elements.pop();
	return best_item;
}
};
