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
	TEST(Astar, Node_Position)
	{
	}

	TEST(Astar, Map_AddNode)
	{
	}

	TEST(Astar, Map_FindPath)
	{
		Node node0(maths::Vector2f(1.1f, 2.2f), std::vector<NodeIndex>{1}); // created node0
		Node node1(maths::Vector2f(0.1f, 0.2f), std::vector<NodeIndex>{0}); // created node1
		std::vector<Node> expectedPath{node0, node1};
		std::vector<Node> graph; // created graph
		graph.push_back(node0); // add node0 in graph
		graph.push_back(node1); // add node1 in graph
		std::vector<Node> path = Map::FindPath(node0, node1, graph);
		EXPECT_TRUE(std::equal(path.begin(), path.end(), expectedPath.begin()));
		std::reverse(expectedPath.begin(), expectedPath.end());
		EXPECT_FALSE(std::equal(path.begin(), path.end(), expectedPath.begin()));
	}

	TEST(Astar, Astar_PriorityQueue)
	{
	}
	
} // namespace astar
