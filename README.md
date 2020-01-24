# Signals

## Why Yet Another Signal-Slot Library?
This library is optimized for video games (and probably other low-latency applications as well). Interestingly, even though the observer pattern is generally useful, it has never been standardized in C++, which leads to the never-ending attempts at improvements by curious people. I view it as a good thing, because if `boost::signals2` was standardized, [it would have been a disaster for real-time applications](https://stackoverflow.com/questions/22416860/is-boostsignals2-overkill-for-simple-applications).

There are many similar libraries - such as [jl_signal](http://hoyvinglavin.com/2012/08/06/jl_signal/), [nuclex signal/slots](http://blog.nuclex-games.com/2019/10/nuclex-signal-slot-benchmarks/) and [several dozens more](https://github.com/NoAvailableAlias/signal-slot-benchmarks). My work is based on a previous [research](https://github.com/TheWisp/ImpossiblyFastEventCPP17) which focused on the syntax and performance improvements brought by a C++17 feature - `template<auto>`. This library is a combination of modern C++ exploration, system programming and data-structure design. It aims to become feature-complete like `boost::signals`, yet extremely light-weight - both run time and memory footprint - in order to replace _interface_ or `std::function` based callbacks.

## Design Choices

### Direct (Blocking) Calls
In game systems, the logic flow often consists of many fast and weakly ordered function calls. 
No thread safety overhead

### Optimized for Emission
Latency is the bottleneck.

### O(1) Connection and Disconnection
In a dynamic world, receivers (slots) are often frequently created and destroyed. A linear search removal algorithm can easily become the performance bottleneck, especially when a large number of slots all disconnect at the same time. Removing by swapping with the end mitigates the problem, but the overall time spent removing N slots with a linear search would still be O(N^2). In this library, a slot is removed by marking its index unused, which then gets skipped and cleaned up in the next emission. Benchmarks have shown that the overhead is dominated by memory accessing (cache misses), rather than checking for null (pipeline stalling).

### Safe Recursion and Modification While Iterating
Just like direct function calls, recursions can naturally emerge from complex and dynamic behaviors. Furthermore, the signals and slots may be side-effected by their own results!

## Usage
Include the single header, `signals.hpp`.

### Basic Usage
```cpp
void on_update(float delta)
{
	std::cout << "This is a function callback";
}

class my_class
{
public:
	void on_update(float delta){ std::cout << "This is a member function callback"; }
};

int main()
{
	fteng::signal<void(float delta)> update;

	//connects a lambda callback
	update.connect([](float delta) { std::cout << "This is a lambda callback"; });

	//connects a function callback
	update.connect(on_update);

	//connects an object's member function
	my_class* my_obj = new my_class;
	update.connect<&my_class::on_update>(my_obj);

	//emit
	update(3.14f);

	delete my_obj;
}
```

### Connection Management
```cpp
// game.h
class game { /*...*/ };
fteng::signal<void(const game& instance)> game_created;

// subsystem.cpp
class subsystem
{
	//Connects a signal with a lambda capturing 'this'
	//Saves the connection which gets destructed when this object is deleted.
	fteng::connection on_game_created = game_created.connect([this](const game& instance)
	{
		std::cout << "Game is created, now we can create other systems!\n";
	});

	//Connects a signal with a member function
	//Saves the connection which gets destructed when this object is deleted.
	fteng::connection on_game_created2 = game_created.connect<&subsystem::on_game_created_method>(this);
	void on_game_created_method(const game& instance)
	{
		std::cout << "Game is created, now we can create other systems!\n";
	};
};
static subsystem subsystem1;

// Somewhere else
int main()
{
	game game_instance;
	game_created(game_instance); //notifies each subsystem
}
```

## Benchmark

Xeon E3-1275 V2 @ 3.90 GHz 16.0 GB RAM

|                                  | Signal (member func) | Signal (lambda) | Virtual Call | Sig(mem func) % of Virtual | Sig(lambda) % of Virtual |
| -------------------------------- | -------------------- | --------------- | ------------ | -------------------------- | ------------------------ |
| SAME class, SEQUENTIAL objects   | 303 us               | 335 us          | 482 us       | 62%                        | 69%                      |
| SAME class, RANDOM objects       | 307 us               | 354 us          | 1336 us      | 22%                        | 26%                      |
| RANDOM class, SEQUENTIAL objects | 990 us               | 995 us          | 1288 us      | 76%                        | 77%                      |
| RANDOM class, RANDOM objects     | 998 us               | 1009 us         | 1795 us      | 55%                        | 56%                      |
