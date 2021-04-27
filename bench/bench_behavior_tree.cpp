#include <benchmark/benchmark.h>

#include <vector>
#include <random>
#include <iostream>

#include <behavior_tree.h>

//NodeType nodeType;
//std::vector<NodeIndex> children;

constexpr std::size_t MAX_CHILDREN_NMB = 64;

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
	static void BM_BehaviorTreeHorizontal(benchmark::State& state)
	{
		//Setup
		std::vector<std::unique_ptr<Behavior>> selectorChildren;
		for (int i = 0; i < MAX_CHILDREN_NMB - 1; i++)
		{
			std::vector<std::unique_ptr<Behavior>> sequenceChildren;
			for (int i = 0; i < MAX_CHILDREN_NMB; i++)
			{
				sequenceChildren.push_back(std::make_unique<LeafTest>(Status::kSuccess));
			}
			selectorChildren.push_back(std::make_unique<Sequence>(std::move(sequenceChildren)));
		}
		std::unique_ptr<Selector> s = std::make_unique<Selector>(std::move(selectorChildren));
		for (auto _ : state)
		{
			benchmark::DoNotOptimize(s->Update());
		}
	}
	// Register the function as a benchmark
	BENCHMARK(BM_BehaviorTreeHorizontal);

	static void BM_BehaviorTreeInit(benchmark::State& state)
	{
		for (auto _ : state)
		{
			//Setup
			std::vector<std::unique_ptr<Behavior>> selectorChildren;
			for (int i = 0; i < MAX_CHILDREN_NMB - 1; i++)
			{
				std::vector<std::unique_ptr<Behavior>> sequenceChildren;
				for (int i = 0; i < MAX_CHILDREN_NMB; i++)
				{
					sequenceChildren.push_back(std::make_unique<LeafTest>(Status::kSuccess));
				}
				selectorChildren.push_back(std::make_unique<Sequence>(std::move(sequenceChildren)));
			}
			std::unique_ptr<Selector> s = std::make_unique<Selector>(std::move(selectorChildren));
		
			benchmark::DoNotOptimize(s);
		}
	}
	// Register the function as a benchmark
	BENCHMARK(BM_BehaviorTreeInit);
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
	static void BM_BehaviorTreeHorizontal2(benchmark::State& state)
	{
		BehaviorTree<256000u> bt;

		//Setup
		Behaviors selectorChildren;
		for (int i = 0; i < MAX_CHILDREN_NMB - 1; i++)
		{
			Behaviors sequenceChildren;
			for (int j = 0; j < MAX_CHILDREN_NMB; j++)
			{
				sequenceChildren.push_back (bt.CreateBehavior<LeafTest>(Status::kSuccess));
			}
			selectorChildren.push_back(bt.CreateBehavior<Sequence>(std::move(sequenceChildren)));
		}
		Selector* s = bt.CreateBehavior<Selector>(std::move(selectorChildren));
		for (auto _ : state)
		{
			benchmark::DoNotOptimize(s->Update());
		}
	}
	// Register the function as a benchmark
	BENCHMARK(BM_BehaviorTreeHorizontal2);
	static void BM_BehaviorTreeInit2(benchmark::State& state)
	{
		for (auto _ : state)
		{
			BehaviorTree<256000u> bt;

			//Setup
			Behaviors selectorChildren;
			for (int i = 0; i < MAX_CHILDREN_NMB - 1; i++)
			{
				Behaviors sequenceChildren;
				for (int j = 0; j < MAX_CHILDREN_NMB; j++)
				{
					sequenceChildren.push_back(bt.CreateBehavior<LeafTest>(Status::kSuccess));
				}
				selectorChildren.push_back(bt.CreateBehavior<Sequence>(std::move(sequenceChildren)));
			}
			Selector* s = bt.CreateBehavior<Selector>(std::move(selectorChildren));

			benchmark::DoNotOptimize(bt);
		}
	}
	// Register the function as a benchmark
	BENCHMARK(BM_BehaviorTreeInit2);
}