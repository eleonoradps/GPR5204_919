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

#include "..\include\behavior_tree.h"

// Updates the Behavior's Status.
Status Behavior::CheckStatus() {

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

		Status s = (children_[current_child_index_])->CheckStatus();

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

		Status s = (children_[current_child_index_])->CheckStatus();

		if (s != Status::kFailure) {

			return s;
		}

		++current_child_index_;

		if (current_child_index_ == children_.size())
		{
			return Status::kFailure;
		}
	}
}
