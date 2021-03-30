#pragma once

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

#include <iostream>
#include <vector>


enum class Status {

	kBaseStatus = 0,
	kSuccess,
	kFailure,
	kRunning
};

class Behavior {

public:
	Behavior() : status_(Status::kBaseStatus) {}
	Behavior(const Behavior& other) = delete;
	Behavior(Behavior&& other) noexcept = default;
	Behavior& operator=(const Behavior& other) = delete;
	Behavior& operator=(Behavior&& other) noexcept = default;
	virtual ~Behavior() = default;

	virtual void Initialize() {}
	virtual Status Update() = 0;
	virtual void Terminate() {}

	Status CheckStatus();
	Status status() const;

private:
	Status status_;
};

using Behaviors = std::vector<std::unique_ptr<Behavior>>;

class Decorator : public Behavior {

public:
	Decorator(std::unique_ptr<Behavior> child) : child_(std::move(child)) {}
protected:
	std::unique_ptr<Behavior> child_;
};

class Composite : public Behavior {

public:
	Composite(Behaviors children) : children_(std::move(children)) {}
	std::size_t currentChildIndex() const { return current_child_index_; }
protected:
	Behaviors children_;
	std::size_t current_child_index_ = 0;
};

class Sequence : public Composite {

public:
	using Composite::Composite;
	void Initialize() override;
	Status Update() override;
};

class Selector : public Composite {

public:
	using Composite::Composite;
	void Initialize() override;
	Status Update() override;
};
