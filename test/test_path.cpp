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

#include <gtest/gtest.h>
#include "paths/path.h"
#include "paths/inverted_priority_queue.h"

namespace path {

TEST(Astar, Map_FindPath) {
	// A test with 2 nodes and 1 path.
	// Create node0.
	Node node0(maths::Vector2f(0.0f, 0.0f), {1});
	// Create node1.
	Node node1(maths::Vector2f(1.0f, 0.0f), {0});
	// Instantiate map.
	Map map;
	// Create expected path.
	std::vector<NodeIndex> expected_path{0, 1};
	// Add node0 in graph.
	map.AddNode(node0);
	// Add node1 in graph.
	map.AddNode(node1);
	// Find the lowest cost path.
	std::vector<NodeIndex> path = map.FindPath(0, 1);
	// Check if the path is the same as the expected path.
	EXPECT_TRUE(std::equal(path.begin(), path.end(), expected_path.begin()));
	// Reverse the expected path.
	std::reverse(expected_path.begin(), expected_path.end());
	// Check if the path is not the same as the expected path.
	EXPECT_FALSE(std::equal(path.begin(), path.end(), expected_path.begin()));

	// A test with 5 nodes and 2 path going into 1 path.
	node0 = Node(maths::Vector2f(0.0f, 0.0f), {1, 3});
	node1 = Node(maths::Vector2f(2.0f, 2.0f), {0, 2});
	Node node2(maths::Vector2f(5.0f, 2.0f), {1, 3, 4});
	Node node3(maths::Vector2f(4.0f, -2.0f), {0, 2});
	Node node4(maths::Vector2f(8.0f, 0.0f), {2});
	map.Reset();
	expected_path = {0, 1, 2, 4};
	map.AddNode(node0);
	map.AddNode(node1);
	map.AddNode(node2);
	map.AddNode(node3);
	map.AddNode(node4);
	path = map.FindPath(0, 4);
	EXPECT_TRUE(std::equal(path.begin(), path.end(), expected_path.begin()));
	std::reverse(expected_path.begin(), expected_path.end());
	EXPECT_FALSE(std::equal(path.begin(), path.end(), expected_path.begin()));

	// A test with 5 node and 2 path.
	node0 = Node(maths::Vector2f(0.0f, 0.0f), {1, 3});
	node1 = Node(maths::Vector2f(0.5f, 1.0f), {0, 2});
	node2 = Node(maths::Vector2f(1.5f, 1.0f), {1, 4});
	node3 = Node(maths::Vector2f(1.0f, -1.0f), {0, 4});
	node4 = Node(maths::Vector2f(2.0f, 0.0f), {2, 3});
	map.Reset();
	expected_path = { 0, 3, 4 };
	map.AddNode(node0);
	map.AddNode(node1);
	map.AddNode(node2);
	map.AddNode(node3);
	map.AddNode(node4);
	path = map.FindPath(0, 4);
	EXPECT_TRUE(std::equal(path.begin(), path.end(), expected_path.begin()));
	std::reverse(expected_path.begin(), expected_path.end());
	EXPECT_FALSE(std::equal(path.begin(), path.end(), expected_path.begin()));

	// A test with 5 nodes and 0 path.
	node0 = Node(maths::Vector2f(0.0f, 0.0f), {1, 2});
	node1 = Node(maths::Vector2f(3.0f, 2.0f), {0, 2});
	node2 = Node(maths::Vector2f(4.0f, -1.0f), {0, 1, 3});
	node3 = Node(maths::Vector2f(5.0f, 2.0f), {3});
	node4 = Node(maths::Vector2f(8.0f, 1.0f), {});
	map.Reset();
	std::vector<NodeIndex> empty_path;
	map.AddNode(node0);
	map.AddNode(node1);
	map.AddNode(node2);
	map.AddNode(node3);
	map.AddNode(node4);
	path = map.FindPath(0, 4);
	EXPECT_TRUE(std::equal(path.begin(), path.end(), empty_path.begin()));
}

TEST(Astar, Astar_PriorityQueue) {
	// Check if the queue is empty.
	PriorityQueue<NodeIndex, float> queue;
	EXPECT_TRUE(queue.empty());

	// A test to put an element in the queue.
	NodeIndex node_index = 1;
	NodeIndex node_index_2 = 2;
	float priority = 2.0f;
	float priority_2 = 1.0f;
	queue.put(node_index, priority);
	EXPECT_TRUE(queue.get() == node_index);
	queue.put(node_index, priority);
	EXPECT_FALSE(queue.get() == node_index_2);

	// A test to get the lowest element.
	queue.put(node_index, priority);
	queue.put(node_index_2, priority_2);
	EXPECT_TRUE(queue.get() == node_index_2);
}

}  // namespace astar
