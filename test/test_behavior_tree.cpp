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
#include <behavior_tree.h>

namespace bt
{
	// Test class to mimick a basic Behavior.
	class LeafTest : public Behavior {
	public:
		LeafTest(Status status) : status_(status) {}

		Status Update() override {
			return status_;
		}

	private:
		Status status_;
	};

	TEST(BehaviorTree, CreateTree)
	{

	}

	// Test a successful Sequence.
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

	// Test a failing Sequence.
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

	// Test when no child satisfies the condition.
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

	// Test when one child is successful.
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

	// Test when two children are successful to see if it stops at the first one.
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

	// Test small tree with one Selector and two Sequences.
	TEST(BehaviorTree, Tree) {
		Behaviors children_a;

		children_a.push_back(std::make_unique<LeafTest>(Status::kFailure));
		children_a.push_back(std::make_unique<LeafTest>(Status::kFailure));
		children_a.push_back(std::make_unique<LeafTest>(Status::kFailure));

		Behaviors children_b;

		children_b.push_back(std::make_unique<LeafTest>(Status::kSuccess));
		children_b.push_back(std::make_unique<LeafTest>(Status::kSuccess));
		children_b.push_back(std::make_unique<LeafTest>(Status::kSuccess));

		Behaviors children_c;

		children_c.push_back(std::make_unique<Sequence>(std::move(children_a)));
		children_c.push_back(std::make_unique<Sequence>(std::move(children_b)));

		Selector c(std::move(children_c));

		Status s = c.Update();

		EXPECT_EQ(s, Status::kSuccess);
		EXPECT_EQ(c.currentChildIndex(), 1);
	}
}

namespace bt2
{
	// Test class to mimick a basic Behavior.
	class LeafTest : public Behavior {
	public:
		LeafTest(Status status) : status_(status) {}

		Status Update() override {
			return status_;
		}

	private:
		Status status_;
	};
	// Test small tree with one Selector and two Sequences.
	TEST(BehaviorTree, Tree) {
		BehaviorTree bt;
		Behaviors children_a{};

		children_a[0] = (bt.CreateBehavior<LeafTest>(Status::kFailure));
		children_a[1] = (bt.CreateBehavior<LeafTest>(Status::kFailure));
		children_a[2] = (bt.CreateBehavior<LeafTest>(Status::kFailure));

		Behaviors children_b{};

		children_b[0] = (bt.CreateBehavior<LeafTest>(Status::kSuccess));
		children_b[1] = (bt.CreateBehavior<LeafTest>(Status::kSuccess));
		children_b[2] = (bt.CreateBehavior<LeafTest>(Status::kSuccess));

		Behaviors children_c{};

		children_c [0] = (bt.CreateBehavior<Sequence>(std::move(children_a)));
		children_c [1] = (bt.CreateBehavior<Sequence>(std::move(children_b)));
		

		Selector* c = bt.CreateBehavior<Selector>(std::move(children_c));

		Status s = c->Update();

		EXPECT_EQ(s, Status::kSuccess);
		EXPECT_EQ(c->currentChildIndex(), 1);
	}
}