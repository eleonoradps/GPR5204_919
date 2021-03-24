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

namespace astar
{
	TEST(Astar, Node_Position){
		
	}

	TEST(Astar, Map_AddNode){
		
	}

	TEST(Astar, Map_FindPath)
	{
		// Test 2 nodes -> 1 path
		const Node node0(maths::Vector2f(0.0f, 0.0f), std::vector<NodeIndex>{1}); // Created node0.
		const Node node1(maths::Vector2f(1.0f, 0.0f), std::vector<NodeIndex>{0}); // Created node1.
		Map map; // Instantiate map
		std::vector<NodeIndex> expectedPath{0, 1}; // Created expected path.
		map.AddNode(node0); // Add node0 in graph
		map.AddNode(node1); // Add node1 in graph
		std::vector<NodeIndex> path = map.FindPath(0, 1); // Find the fastest path 
		EXPECT_TRUE(std::equal(path.begin(), path.end(), expectedPath.begin())); // Check if the path is the same as the expected path
		std::reverse(expectedPath.begin(), expectedPath.end()); // Reverse the expetced path
		EXPECT_FALSE(std::equal(path.begin(), path.end(), expectedPath.begin())); // Check if the path is not the same as the expected path
	}

	TEST(Astar, Map_FindPath2)
	{
		// Test 
		Node node0(maths::Vector2f(0.0f, 0.0f), std::vector<NodeIndex>{1, 3});
		Node node1(maths::Vector2f(2.0f, 2.0f), std::vector<NodeIndex>{0, 2});
		Node node2(maths::Vector2f(5.0f, 2.0f), std::vector<NodeIndex>{1, 3, 4});
		Node node3(maths::Vector2f(4.0f, -2.0f), std::vector<NodeIndex>{0, 2});
		Node node4(maths::Vector2f(8.0f, 0.0f), std::vector<NodeIndex>{2});
		Map map;
		std::vector<NodeIndex> expectedPath{ 0, 1, 2, 4 };
		map.AddNode(node0);
		map.AddNode(node1);
		map.AddNode(node2);
		map.AddNode(node3);
		map.AddNode(node4);
		std::vector<NodeIndex> path = map.FindPath(0, 4);
		EXPECT_TRUE(std::equal(path.begin(), path.end(), expectedPath.begin()));
		std::reverse(expectedPath.begin(), expectedPath.end());
		EXPECT_FALSE(std::equal(path.begin(), path.end(), expectedPath.begin()));
	}

	TEST(Astar, Map_FindPath3)
	{
		// Test 
		Node node0(maths::Vector2f(0.0f, 0.0f), std::vector<NodeIndex>{1, 3});
		Node node1(maths::Vector2f(2.0f, 2.0f), std::vector<NodeIndex>{0, 2});
		Node node2(maths::Vector2f(5.0f, 2.0f), std::vector<NodeIndex>{1, 3, 4});
		Node node3(maths::Vector2f(4.0f, -2.0f), std::vector<NodeIndex>{0, 2});
		Node node4(maths::Vector2f(8.0f, 0.0f), std::vector<NodeIndex>{2});
		Map map;
		std::vector<NodeIndex> expectedPath{ 0, 1, 2, 4 };
		map.AddNode(node0);
		map.AddNode(node1);
		map.AddNode(node2);
		map.AddNode(node3);
		map.AddNode(node4);
		std::vector<NodeIndex> path = map.FindPath(0, 4);
		EXPECT_TRUE(std::equal(path.begin(), path.end(), expectedPath.begin()));
		std::reverse(expectedPath.begin(), expectedPath.end());
		EXPECT_FALSE(std::equal(path.begin(), path.end(), expectedPath.begin()));
	}

	TEST(Astar, Astar_PriorityQueue){
		
	}
	
} // namespace astar
