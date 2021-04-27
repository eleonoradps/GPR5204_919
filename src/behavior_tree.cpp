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

#include <behavior_tree.h>

namespace bt
{

	// Updates the Behavior's Status.
	Status Behavior::GetStatus() {
		if (status_ != Status::kRunning) {
			Initialize();
		}

		status_ = Update();

		if (status_ != Status::kRunning) {
			Terminate();
		}
		return status_;
	}

	Status Behavior::status() const {
		return status_;
	}

	std::size_t Composite::currentChildIndex() const {
		return current_child_index_;
	}

	// Sets the base Status of the Behavior.
	// Here sets the children index at 0.
	void Sequence::Initialize() {
		current_child_index_ = 0;
	}

	// This is where the action/behavior is ran. 
	// Returns Status if failure or success.
	Status Sequence::Update() {
		while (true) {
			Status s = (children_[current_child_index_])->GetStatus();

			if (s != Status::kSuccess) {
				return s;
			}

			++current_child_index_;

			if (current_child_index_ == children_.size()) {
				return Status::kSuccess;
			}
		}
	}

	// Sets the base Status of the Behavior.
	// Here sets the children index at 0.
	void Selector::Initialize() {
		current_child_index_ = 0;
	}

	// This is where the action/behavior is ran. 
	// Returns Status if failure or success.
	Status Selector::Update() {
		while (true) {
			Status s = (children_[current_child_index_])->GetStatus();

			if (s != Status::kFailure) {
				return s;
			}

			++current_child_index_;

			if (current_child_index_ == children_.size()) {
				return Status::kFailure;
			}
		}
	}

	//inline void BehaviorTree::AddBehavior() {
	//	behaviors.push_back()
	//}

	BehaviorTree::BehaviorTree()
	{
		sequenceInit_ = [this](NodeIndex currentIndex) {
			auto& sequenceNode = nodes_[currentIndex];
			sequenceNode.status = Status::kInvalid;
			for (auto& childIndex : sequenceNode.children)
			{
				auto& child = nodes_[childIndex];
				child.init(childIndex);
			}
		};
		sequenceUpdate_ = [this](NodeIndex currentIndex) -> Status {
			auto& sequenceNode = nodes_[currentIndex];
			for (auto& childIndex : sequenceNode.children)
			{
				auto& child = nodes_[childIndex];
				switch (child.status)
				{
				case Status::kFailure:
					return Status::kFailure;
				case Status::kSuccess:
					continue;
				case Status::kInvalid:
				case Status::kRunning:
					if (child.status == Status::kInvalid)
					{
						child.init(childIndex);
					}
					Status s = child.update(childIndex);
					switch (s)
					{
					case Status::kFailure:
						sequenceNode.status = Status::kFailure;
						return Status::kFailure;
					case Status::kSuccess:
						continue;
					case Status::kRunning:
						return Status::kRunning;
					}
				}
			}
			sequenceNode.status = Status::kSuccess;
			return Status::kSuccess;

		};

		selectorInit_ = [this](NodeIndex currentIndex) {
			auto& selectorNode = nodes_[currentIndex];
			selectorNode.status = Status::kInvalid;
			for (auto& childIndex : selectorNode.children)
			{
				auto& child = nodes_[childIndex];
				child.init(childIndex);
			}
		};
		selectorUpdate_ = [this](NodeIndex currentIndex) -> Status {
			auto& selectorNode = nodes_[currentIndex];
			for (auto& childIndex : selectorNode.children)
			{
				auto& child = nodes_[childIndex];
				switch (child.status)
				{
				case Status::kFailure:
					return Status::kFailure;
				case Status::kSuccess:
					continue;
				case Status::kInvalid:
				case Status::kRunning:
					if (child.status == Status::kInvalid)
					{
						child.init(childIndex);
					}
					Status s = child.update(childIndex);
					switch (s)
					{
					case Status::kFailure:
						selectorNode.status = Status::kFailure;
						return Status::kFailure;
					case Status::kSuccess:
						continue;
					case Status::kRunning:
						return Status::kRunning;
					}
				}
			}
			selectorNode.status = Status::kSuccess;
			return Status::kSuccess;
		};

		actionInit_ = [this](NodeIndex currentIndex) {
			auto& actionNode = nodes_[currentIndex];
			actionNode.status = Status::kInvalid;
		};
		actionUpdate_ = [this](NodeIndex currentIndex) -> Status {
			auto& actionNode = nodes_[currentIndex];

			actionNode.status = Status::kSuccess;
			return Status::kSuccess;
		};
	}

	NodeIndex BehaviorTree::CreateNode(NodeType nodeType, std::vector<NodeIndex> children)
	{
		BehaviorTreeNode node;
		NodeIndex nodeIndex;
		switch (nodeType)
		{
		case NodeType::SELECTOR:
			node.init = selectorInit_;
			node.update = selectorUpdate_;
			break;
		case NodeType::SEQUENCE:
			node.init = sequenceInit_;
			node.update = sequenceUpdate_;
			break;
		case NodeType::ACTION:
			node.init = actionInit_;
			node.update = actionUpdate_;
			break;
		}
		node.children = children;
		nodes_.push_back(node);
		nodeIndex = nodes_.size() - 1;
		return nodeIndex;
	}

	void BehaviorTree::Init()
	{
		nodes_[0].init(0);
	}

	void BehaviorTree::Update()
	{
		nodes_[0].update(0);
	}
}

namespace bt2
{
	// Updates the Behavior's Status.
	Status Behavior::GetStatus() {
		if (status_ != Status::kRunning) {
			Initialize();
		}

		status_ = Update();

		if (status_ != Status::kRunning) {
			Terminate();
		}
		return status_;
	}

	Status Behavior::status() const {
		return status_;
	}

	std::size_t Composite::currentChildIndex() const {
		return current_child_index_;
	}

	// Sets the base Status of the Behavior.
	// Here sets the children index at 0.
	void Sequence::Initialize() {
		current_child_index_ = 0;
	}

	// This is where the action/behavior is ran. 
	// Returns Status if failure or success.
	Status Sequence::Update() {
		while (true) {
			Status s = (children_[current_child_index_])->GetStatus();

			if (s != Status::kSuccess) {
				return s;
			}

			++current_child_index_;

			if (current_child_index_ == children_.size()) {
				return Status::kSuccess;
			}
		}
	}

	// Sets the base Status of the Behavior.
	// Here sets the children index at 0.
	void Selector::Initialize()  {
		current_child_index_ = 0;
	}

	// This is where the action/behavior is ran. 
	// Returns Status if failure or success.
	Status Selector::Update()  {
		while (true) {
			Status s = (children_[current_child_index_])->GetStatus();

			if (s != Status::kFailure) {
				return s;
			}

			++current_child_index_;

			if (current_child_index_ == children_.size()) {
				return Status::kFailure;
			}
		}
	}

}