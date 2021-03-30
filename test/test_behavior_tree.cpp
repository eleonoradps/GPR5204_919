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
#include "behavior_tree.h"

class LeafTest : public Behavior {

public:
	LeafTest(Status status) : status_(status) {}

	Status Update() override
	{
		return status_;
	}

private:
	Status status_;
};

TEST(Sequence, SequenceAllSuccess) {

	Behaviors children;

	children.push_back(std::make_unique<LeafTest>(Status::kSuccess));
	children.push_back(std::make_unique<LeafTest>(Status::kSuccess));
	children.push_back(std::make_unique<LeafTest>(Status::kSuccess));

	Sequence a(std::move(children));

	Status s = a.Update();

	EXPECT_EQ(s, Status::kSuccess);
	EXPECT_EQ(a.currentChildIndex(), 3);
}

TEST(Sequence, SequenceAllFailure) {

	Behaviors children;

	children.push_back(std::make_unique<LeafTest>(Status::kFailure));
	children.push_back(std::make_unique<LeafTest>(Status::kFailure));
	children.push_back(std::make_unique<LeafTest>(Status::kFailure));

	Sequence a(std::move(children));

	Status s = a.Update();

	EXPECT_EQ(s, Status::kFailure);
	EXPECT_EQ(a.currentChildIndex(), 0);
}

TEST(Selector, SelectorAllFailure) {

	Behaviors children;

	children.push_back(std::make_unique<LeafTest>(Status::kFailure));
	children.push_back(std::make_unique<LeafTest>(Status::kFailure));
	children.push_back(std::make_unique<LeafTest>(Status::kFailure));

	Selector a(std::move(children));

	Status s = a.Update();

	EXPECT_EQ(s, Status::kFailure);
	EXPECT_EQ(a.currentChildIndex(), 3);
}

TEST(Selector, SelectorOneSuccess) {

	Behaviors children;

	children.push_back(std::make_unique<LeafTest>(Status::kFailure));
	children.push_back(std::make_unique<LeafTest>(Status::kSuccess));
	children.push_back(std::make_unique<LeafTest>(Status::kFailure));

	Selector a(std::move(children));

	Status s = a.Update();

	EXPECT_EQ(s, Status::kSuccess);
	EXPECT_EQ(a.currentChildIndex(), 1);
}

TEST(Selector, SelectorTwoSuccess) {

	Behaviors children;

	children.push_back(std::make_unique<LeafTest>(Status::kFailure));
	children.push_back(std::make_unique<LeafTest>(Status::kSuccess));
	children.push_back(std::make_unique<LeafTest>(Status::kSuccess));

	Selector a(std::move(children));

	Status s = a.Update();

	EXPECT_EQ(s, Status::kSuccess);
	EXPECT_EQ(a.currentChildIndex(), 1);
}