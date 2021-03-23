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
	virtual ~Behavior() {}

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
	void AddChild(std::unique_ptr<Behavior> child);
	void RemoveChild(std::unique_ptr<Behavior>);
	void EmptyChildren();
protected:
	Behaviors children_;
};

class Sequence : public Composite {

protected:
	virtual ~Sequence() {}
	virtual void Initialize();
	virtual Status Update();

	Behaviors::iterator current_child;
};

class Selector : public Composite {

protected:
	virtual ~Selector() {}
	virtual void Initialize();
	virtual Status Update();

	Behaviors::iterator current_child;
};
