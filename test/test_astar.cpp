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
#include "astar.h"
#include "inverted_priority_queue.h"

namespace astar {
TEST(Astar, Map_AddNode) {
	// Test adding node in the graph.
	Map map;
	const Node node(maths::Vector2f(0.0f, 0.0f), std::vector<NodeIndex>{});
	const Node false_node(maths::Vector2f(2.0f, 6.0f), std::vector<NodeIndex>{});
	map.AddNode(node);
	std::vector<Node> graph = map.GetGraph();
	EXPECT_TRUE(node == graph[0]);
	EXPECT_FALSE(false_node == graph[0]);
}

TEST(Astar, Map_FindPath) {
	// Test 2 nodes & 1 path.
	const Node node0(maths::Vector2f(0.0f, 0.0f), std::vector<NodeIndex>{1}); // Created node0.
	const Node node1(maths::Vector2f(1.0f, 0.0f), std::vector<NodeIndex>{0}); // Created node1.
	Map map; // Instantiate map
	std::vector<NodeIndex> expected_path{0, 1}; // Created expected path.
	map.AddNode(node0); // Add node0 in graph.
	map.AddNode(node1); // Add node1 in graph.
	std::vector<NodeIndex> path = map.FindPath(0, 1); // Find the lowest cost path .
	EXPECT_TRUE(std::equal(path.begin(), path.end(), expected_path.begin())); // Check if the path is the same as the expected path.
	std::reverse(expected_path.begin(), expected_path.end()); // Reverse the expected path.
	EXPECT_FALSE(std::equal(path.begin(), path.end(), expected_path.begin())); // Check if the path is not the same as the expected path.
}

TEST(Astar, Map_FindPath2) {
	// Test 5 nodes & 2 path -> 1 path.
	Node node0(maths::Vector2f(0.0f, 0.0f), std::vector<NodeIndex>{1, 3});
	Node node1(maths::Vector2f(2.0f, 2.0f), std::vector<NodeIndex>{0, 2});
	Node node2(maths::Vector2f(5.0f, 2.0f), std::vector<NodeIndex>{1, 3, 4});
	Node node3(maths::Vector2f(4.0f, -2.0f), std::vector<NodeIndex>{0, 2});
	Node node4(maths::Vector2f(8.0f, 0.0f), std::vector<NodeIndex>{2});
	Map map;
	std::vector<NodeIndex> expected_path{0, 1, 2, 4};
	map.AddNode(node0);
	map.AddNode(node1);
	map.AddNode(node2);
	map.AddNode(node3);
	map.AddNode(node4);
	std::vector<NodeIndex> path = map.FindPath(0, 4);
	EXPECT_TRUE(std::equal(path.begin(), path.end(), expected_path.begin()));
	std::reverse(expected_path.begin(), expected_path.end());
	EXPECT_FALSE(std::equal(path.begin(), path.end(), expected_path.begin()));
}

TEST(Astar, Map_FindPath3) {
	// Test 5 node & 2 path.
	Node node0(maths::Vector2f(0.0f, 0.0f), std::vector<NodeIndex>{1, 3});
	Node node1(maths::Vector2f(0.5f, 1.0f), std::vector<NodeIndex>{0, 2});
	Node node2(maths::Vector2f(1.5f, 1.0f), std::vector<NodeIndex>{1, 4});
	Node node3(maths::Vector2f(1.0f, -1.0f), std::vector<NodeIndex>{0, 4});
	Node node4(maths::Vector2f(2.0f, 0.0f), std::vector<NodeIndex>{2, 3});
	Map map;
	std::vector<NodeIndex> expected_path{0, 3, 4};
	map.AddNode(node0);
	map.AddNode(node1);
	map.AddNode(node2);
	map.AddNode(node3);
	map.AddNode(node4);
	std::vector<NodeIndex> path = map.FindPath(0, 4);
	EXPECT_TRUE(std::equal(path.begin(), path.end(), expected_path.begin()));
	std::reverse(expected_path.begin(), expected_path.end());
	EXPECT_FALSE(std::equal(path.begin(), path.end(), expected_path.begin()));
}

TEST(Astar, Map_FindPath4) {
	// Test 5 nodes & 0 path.
	Node node0(maths::Vector2f(0.0f, 0.0f), std::vector<NodeIndex>{1, 2});
	Node node1(maths::Vector2f(3.0f, 2.0f), std::vector<NodeIndex>{0, 2});
	Node node2(maths::Vector2f(4.0f, -1.0f), std::vector<NodeIndex>{0, 1, 3});
	Node node3(maths::Vector2f(5.0f, 2.0f), std::vector<NodeIndex>{3});
	Node node4(maths::Vector2f(8.0f, 1.0f), std::vector<NodeIndex>{});
	Map map;
	std::vector<NodeIndex> expected_path;
	map.AddNode(node0);
	map.AddNode(node1);
	map.AddNode(node2);
	map.AddNode(node3);
	map.AddNode(node4);
	std::vector<NodeIndex> path = map.FindPath(0, 4);
	EXPECT_TRUE(std::equal(path.begin(), path.end(), expected_path.begin()));
}

TEST(Astar, Astar_PriorityQueue) {
	// Check if the queue is empty.
	PriorityQueue<NodeIndex, float> queue;
	EXPECT_TRUE(queue.empty());

	// Test to put an element in the queue.
	NodeIndex node_index = 1;
	NodeIndex node_index_2 = 2;
	float priority = 2.0f;
	float priority_2 = 1.0f;
	queue.put(node_index, priority);
	EXPECT_TRUE(queue.get() == node_index);
	queue.put(node_index, priority);
	EXPECT_FALSE(queue.get() == node_index_2);

	// Test to get the lowest element.
	queue.put(node_index, priority);
	queue.put(node_index_2, priority_2);
	EXPECT_TRUE(queue.get() == node_index_2);
}
} // namespace astar
