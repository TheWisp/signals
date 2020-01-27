#include <cassert>
#include "signals.hpp"
#include <functional>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <random>
#include <array>
#include <vector>
#include <chrono>
#include <memory>

using namespace fteng;

struct Foo
{
	signal<void(float)> sig;
};

struct Bar
{
	void onSig(float delta)
	{
		total += delta;
	}

	connection conn;

	Bar(const Foo& foo)
	{
		conn = foo.sig.connect<&Bar::onSig>(this);
	}

	volatile float total = 0.f;
};

namespace ReentranceTest
{
	struct Foo
	{
		signal<void(int count)> sig;
	};

	struct Bar
	{
		void onSig(int count);

		connection conn;
		Foo* m_foo;
		int x = 0;
		bool flag;

		Bar(Foo& foo, bool flag = true)
			: flag(flag)
		{
			conn = foo.sig.connect<&Bar::onSig>(this);
			m_foo = &foo;
		}
	};

	std::vector<std::unique_ptr<Bar>> dyn_connections;

	void test()
	{
		dyn_connections.clear();

		Foo foo;
		for (int i = 0, n = rand() % 10; i < n; i++)
			dyn_connections.emplace_back(new Bar(foo, false));

		Bar bar(foo);

		foo.sig(30);
		assert(bar.x == 30);
		foo.sig(30);
		assert(bar.x == 60);
	}

	inline void Bar::onSig(int count)
	{
		if (flag) {
			if (count > 0) {
				this->x++;

				//purposely add or remove listeners
				if (rand() & 1)
					dyn_connections.emplace_back(new Bar(*m_foo, false));
				else if (!dyn_connections.empty())
					dyn_connections.erase(dyn_connections.begin() + rand() % dyn_connections.size());
				m_foo->sig(count - 1);
			}
		}
	}
}

namespace FreeFuncTest
{
	signal<void(int count)> sig;
	int c = 0;
	void onSig(int count) 
	{
		c += count;
	}

	connection conn;
	void test()
	{
		conn = sig.connect<onSig>();
		sig(33);
		assert(c == 33);
	}
}

namespace LambdaTest
{
	signal<void(int count)> sig;
	connection conn;
	int glob_c = 0;

	void test()
	{
		int c = 0;
		//small capture lambda test
		conn = sig.connect([&](int count) { c += count; });
		sig(33);
		assert(c == 33);
		
		//non-captured lambda test
		conn = sig.connect([](int count) { 
			glob_c += count; 
		});
		sig(32);
		assert(glob_c == 32);

		int x = 100;
		int y = 100;
		//large lambda test
		auto large_lambda = [&](int count) {
			x += y + count;
		};
		static_assert(std::is_trivially_destructible_v<decltype(large_lambda)>);
		static_assert(sizeof(large_lambda) > 8);
		conn = sig.connect(large_lambda);
		sig(32);
		assert(x == 232);

		auto large_lambda_2 = [&, v = std::vector{1,2,3}](int count) {
			x = *std::max_element(v.begin(), v.end()) + count;
		};
		static_assert(!std::is_trivially_destructible_v<decltype(large_lambda_2)>);
		conn = sig.connect(large_lambda_2);
		sig(32);
		assert(x == 35);

		//generic lambda test
		conn = sig.connect([](auto&& ...a) 
			{
				static_assert(sizeof... (a) == 1);
			});

		//captured 0-memory lambda test
		conn = sig.connect([y = 0](int count) mutable {
			y += count; //no sideeffect, but should work
		});
		sig(10);
	}
}

namespace BlockingConnection
{
	signal<void(int count)> sig;

	void test()
	{
		int x = 0;

		connection conn = sig.connect([&](int count) {
			x += count;
			conn.block();
			sig(count);
			conn.unblock();
		});

		sig(33);
		assert(x == 33);
		sig(33);
		assert(x == 66);
	}
}

namespace FunctorTest
{
	struct Functor
	{
		int val = 0;
		void operator()(int x)
		{
			val += x;
		}
	};

	void test()
	{
		Functor o;
		signal<void(int)> sig;
		sig.connect(o);
		sig(10);
		assert(o.val == 10);
	}
}

void test()
{
	//basic usage, class
	{
		// Being able to connect, disconnect and emit signal owned by a const object demonstrates 
		// that the signal is considered side-effect-free to its owner. All its data are mutable.
		const Foo foo; 
		Bar bar(foo);
		foo.sig(3.14f);
		assert(bar.total == 3.14f);
		foo.sig(3.14f);
		assert(bar.total == 6.28f);
	}

	//basic usage, not saving connection
	{
		signal<void(int)> sig;
		int x = 0;
		sig.connect([&](int y) { x += y; });
		sig(3);
		assert(x == 3);
	}

	//scope safety (connection)
	{
		Foo foo;
		{
			Bar bar1(foo), * bar3;
			{
				Bar bar2(foo);
				bar3 = new Bar(foo);
				foo.sig(1.f);
				assert(bar1.total == 1);
				assert(bar2.total == 1);
				assert(bar3->total == 1);
			}
			foo.sig(1.f);
			assert(bar1.total == 2);
			assert(bar3->total == 2);
		}
		foo.sig(1.f);
	}

	//reentrance and modification safety
	ReentranceTest::test();

	//basic usage, free function
	FreeFuncTest::test();

	//basic usage, lambda
	LambdaTest::test();

	//blocking connection
	BlockingConnection::test();

	//lambda and functor instance test
	FunctorTest::test();
}

namespace Bench
{
#include "Bench.h"
}

int main()
{
	test();
	Bench::bench();
}