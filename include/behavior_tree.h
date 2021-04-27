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
#include <array>
#include <memory>
#include <functional>
#include <custom_allocator.h>

namespace bt
{
	// Describes the States of the Behavior class.
	enum class Status {
		kInvalid = 0,
		kSuccess,
		kFailure,
		kRunning
	};

	//class BehaviorTree {
	//	std::vector<Behavior> behaviors;
	//
	//	void AddBehavior(int leafTest1);
	//};
	// Abstract class, it is the basis of every other class in a Behavior Tree.
	class Behavior {
	public:
		// Default constructor.
		Behavior() : status_(Status::kInvalid) {}

		// Copy constructor.
		Behavior(const Behavior& other) = delete;

		// Move constructor.
		Behavior(Behavior&& other) noexcept = default;

		// Copy assignment.
		Behavior& operator=(const Behavior& other) = delete;

		// Move assignment.
		Behavior& operator=(Behavior&& other) noexcept = default;

		// Virtual destructor.
		virtual ~Behavior() = default;

		// Sets the base Status of the Behavior.
		virtual void Initialize() {}

		// This is where the action/behavior is ran. 
		// Returns Status if failure or success.
		virtual Status Update() = 0;

		// Is called when the Update function signals the action 
		// is no longer ongoing.
		// In the Terminate function, we can put what we need the Behavior to do
		// when the action is done.
		virtual void Terminate() {}

		// Updates the Behavior's Status.
		Status GetStatus();
		Status status() const;

	private:
		Status status_;
	};

	using Behaviors = std::vector<std::unique_ptr<Behavior>>;

	// A node class that only has one leaf/child.
	class Decorator : public Behavior {
	public:
		Decorator(std::unique_ptr<Behavior> child) : child_(std::move(child)) {} // std behavior *
	protected:
		std::unique_ptr<Behavior> child_;
	};

	// A node class with multiple leaves/children.
	class Composite : public Behavior {
	public:
		Composite(Behaviors children) : children_(std::move(children)) {}
		std::size_t currentChildIndex() const;
	protected:
		Behaviors children_;
		std::size_t current_child_index_ = 0;
	};

	// A Composite class that executes all its leaves/children one after another.
	class Sequence : public Composite {
	public:
		using Composite::Composite;

		// Sets the base Status of the Behavior.
		// Here sets the children index at 0.
		void Initialize() override;

		// This is where the action/behavior is ran. 
		// Returns Status if failure or success.
		Status Update() override;
	};

	// A Composite class that loops through its leaves/children and
	// selects the first that satisfies its condition.
	class Selector : public Composite {
	public:
		using Composite::Composite;

		// Sets the base Status of the Behavior.
		// Here sets the children index at 0.
		void Initialize() override;

		// This is where the action/behavior is ran. 
		// Returns Status if failure or success.
		Status Update() override;
	};

	enum class NodeType {
		SELECTOR,
		SEQUENCE,
		ACTION
	};

	using NodeIndex = std::uint32_t;
	struct BehaviorTreeNode
	{
		std::vector<NodeIndex> children;
		std::function<void(NodeIndex)> init;
		std::function<Status(NodeIndex)> update;
		Status status = Status::kInvalid;
	};

	class BehaviorTree
	{
	public:
		BehaviorTree();
		NodeIndex CreateNode(NodeType nodeType, std::vector<NodeIndex> children);
		void Init();
		void Update();

		BehaviorTreeNode& GetNode(NodeIndex index);
	private:
		NodeIndex currentIndex = 0;
		std::vector<BehaviorTreeNode> nodes_;

		std::function<void(NodeIndex)> selectorInit_;
		std::function<Status(NodeIndex)> selectorUpdate_;
		std::function<void(NodeIndex)> sequenceInit_;
		std::function<Status(NodeIndex)> sequenceUpdate_;
		std::function<void(NodeIndex)> actionInit_;
		std::function<Status(NodeIndex)> actionUpdate_;
	};

}

namespace bt2
{
	// Describes the States of the Behavior class.
	enum class Status {
		kInvalid = 0,
		kSuccess,
		kFailure,
		kRunning
	};
	// args = argument constructeur de T
	// variadic args = argument variant
	// pas de new appeler, fais la différence avec std make unique

	template<std::size_t AllocSize = 65536u>
	class BehaviorTree
	{
	public:
		BehaviorTree() : buffer_((unsigned char*)malloc(AllocSize)), allocator_(AllocSize, buffer_)
		{
		}
		~BehaviorTree()
		{
			allocator_.Clear();
			free(buffer_);
		}
		template<typename T, typename... Args>
		T* CreateBehavior(Args... args)
		{
			auto* ptr = (T*)allocator_.Allocate(sizeof(T), alignof(T));
			std::construct_at(ptr, args...);
			return ptr;

		}
	private:
		unsigned char* buffer_; // memory
		neko::LinearAllocator allocator_; // manager of memory
	};

	//class BehaviorTree {
	//	std::vector<Behavior> behaviors;
	//
	//	void AddBehavior(int leafTest1);
	//};
	// Abstract class, it is the basis of every other class in a Behavior Tree.
	class Behavior {
	public:
		// Default constructor.
		Behavior() : status_(Status::kInvalid) {}

		// Copy constructor.
		Behavior(const Behavior& other) = delete;

		// Move constructor.
		Behavior(Behavior&& other) noexcept = default;

		// Copy assignment.
		Behavior& operator=(const Behavior& other) = delete;

		// Move assignment.
		Behavior& operator=(Behavior&& other) noexcept = default;

		// Virtual destructor.
		virtual ~Behavior() = default;

		// Sets the base Status of the Behavior.
		virtual void Initialize()  {}

		// This is where the action/behavior is ran. 
		// Returns Status if failure or success.
		virtual Status Update()  = 0;

		// Is called when the Update function signals the action 
		// is no longer ongoing.
		// In the Terminate function, we can put what we need the Behavior to do
		// when the action is done.
		virtual void Terminate()  {}

		// Updates the Behavior's Status.
		Status GetStatus();
		Status status() const;

	private:
		Status status_;
	};

	using Behaviors = std::vector<Behavior*>; //array of max children nmb

	// A node class that only has one leaf/child.
	class Decorator : public Behavior {
	public:
		Decorator(std::unique_ptr<Behavior> child) : child_(std::move(child)) {} // std behavior *
	protected:
		std::unique_ptr<Behavior> child_; //Behavior*
	};

	// A node class with multiple leaves/children.
	class Composite : public Behavior {
	public:
		Composite(Behaviors children) : children_(std::move(children)) {}
		std::size_t currentChildIndex() const;
	protected:
		Behaviors children_;
		std::size_t current_child_index_ = 0;
	};

	// A Composite class that executes all its leaves/children one after another.
	class Sequence : public Composite {
	public:
		using Composite::Composite;

		// Sets the base Status of the Behavior.
		// Here sets the children index at 0.
		void Initialize()  override;

		// This is where the action/behavior is ran. 
		// Returns Status if failure or success.
		Status Update()  override;
	};

	// A Composite class that loops through its leaves/children and
	// selects the first that satisfies its condition.
	class Selector : public Composite {
	public:
		using Composite::Composite;

		// Sets the base Status of the Behavior.
		// Here sets the children index at 0.
		void Initialize()  override;

		// This is where the action/behavior is ran. 
		// Returns Status if failure or success.
		Status Update()  override;
	};
}