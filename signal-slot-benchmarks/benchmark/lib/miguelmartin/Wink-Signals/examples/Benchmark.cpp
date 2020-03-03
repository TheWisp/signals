///
/// Wink Signals - Benchmark
/// Copyright (C) 2013-2014 Miguel Martin (miguel@miguel-martin.com)
///
///
/// This software is provided 'as-is', without any express or implied warranty.
/// In no event will the authors be held liable for any damages arising from the
/// use of this software.
///
/// Permission is hereby granted, free of charge, to any person
/// obtaining a copy of this software and associated documentation files (miguel@miguel-martin.com),
/// to deal in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// 1. The origin of this software must not be misrepresented;
///    you must not claim that you wrote the original software.
///    If you use this software in a product, an acknowledgment
///    in the product documentation would be appreciated but is not required.
///
/// 2. Altered source versions must be plainly marked as such,
///	   and must not be misrepresented as being the original software.
///
/// 3. The above copyright notice and this permission notice shall be included in
///    all copies or substantial portions of the Software.
///

// Compiler-check
#ifdef _MSC_VER
// if we're not compiling with VS2010 (or less)
// we wont do a proper benchmark
#	if !(_MSC_VER <= 1600)
#		define DO_PROPER_BENCHMARK
#	endif // (_MSC_VER <= 1600)
		  // otherwise, were not using VS
#else
// let's do a proper benchmark
#		define DO_PROPER_BENCHMARK
#endif // _MSC_VER

#include <iostream>
#include <cstdlib>
#include <cmath>

#ifdef DO_PROPER_BENCHMARK
#	include <chrono> // to measure time
#endif // DO_PROPER_BENCHMARK

#include <wink/signal.hpp>
#include <wink/event_queue.hpp>

// Class to handle events
struct EventHandler
{
	/// Handles an event that sends an integer
	void handleEvent(int x)
	{
		// if we're not in release mode
#ifndef DO_PROPER_BENCHMARK
		std::cout << "Event recieved x = " << x << '\n';
#endif // DO_PROPER_BENCHMARK
		
		int y = rand();
		// simple computation
		int z = std::sqrt(y) * y + x;
	}
	
	
	/// Different function for a queue, since it uses a different prototype,
	/// but it does contain the same implemenation
	void queueHandleEvent(const int& x)
	{
		// if we're not in release mode
#ifndef DO_PROPER_BENCHMARK
		std::cout << "Event recieved x = " << x << '\n';
#endif // DO_PROPER_BENCHMARK
		
		int y = rand();
		// simple computation
		int z = std::sqrt(y) * y + x;
	}
};

#ifdef DO_PROPER_BENCHMARK
/// \return The time since epoch, in seconds
double GetTimeNow()
{
	using std::chrono::duration;
	using std::chrono::duration_cast;
	using std::chrono::high_resolution_clock;
	
	return duration_cast<duration<double, std::ratio<1> > >(high_resolution_clock::now().time_since_epoch()).count();
}
#endif // DO_PROPER_BENCHMARK

/// Prints the time taken to do a task
void printTimeTaken(double start, double end)
{
	std::cout << "Took: " << (end - start) << " seconds\n";
}

template <class T0, class T1>
struct Test
{
	
};

int main(int argc, char* argv[])
{
	// the amount of times to send the event out
	const int AMOUNT_OF_TIMES_TO_SEND_EVENT
#ifdef DO_PROPER_BENCHMARK
	= 100000000;
#else
	= 5; // don't want to print out a million things on the screen
#endif // DO_PROPER_BENCHMARK
	
	
	double start = 0;	// starting time
	double end = 0;		// ending time
	
	// create some integers to send
	std::vector<int> numbersToSend((rand() % 100) + 1);
	
	// randomize the integers
	for(std::vector<int>::iterator i = numbersToSend.begin(); i != numbersToSend.end(); ++i)
	{
		*i = rand();
	}
	
	
#ifdef DO_PROPER_BENCHMARK
	std::cout << "BENCH MARK TO SEND " << AMOUNT_OF_TIMES_TO_SEND_EVENT << " EVENTS\n\n";
	
	{
		EventHandler handler;
		
		std::cout << "Using regular function calls to handle events:\n";
		start = GetTimeNow();
		for(int i = 0; i < AMOUNT_OF_TIMES_TO_SEND_EVENT; ++i)
		{
			for(std::vector<int>::iterator i = numbersToSend.begin(); i != numbersToSend.end(); ++i)
			{
				// call the event handler directly
				// (this will probably be in-lined, with optimization)
				
				handler.handleEvent(*i);
			}
		}
		end = GetTimeNow();
		// print the time taken
		printTimeTaken(start, end);
	}
	
#endif // DO_PROPER_BENCHMARK
	
	{
		typedef wink::signal<wink::slot<void (int)> > signal;
		EventHandler handler;
		signal sender;
		
		sender.connect(&handler, &EventHandler::handleEvent);
		
		std::cout << "Using signal<slot<void(int)>> to handle events:\n";
		
#ifdef DO_PROPER_BENCHMARK
		start = GetTimeNow();
#endif // DO_PROPER_BENCHMARK
		
		for(int i = 0; i < AMOUNT_OF_TIMES_TO_SEND_EVENT; ++i)
		{
			for(std::vector<int>::iterator i = numbersToSend.begin(); i != numbersToSend.end(); ++i)
			{
				// emit the event
				sender(*i);
			}
		}
		
#ifdef DO_PROPER_BENCHMARK
		end = GetTimeNow();
		
		// print the time taken
		printTimeTaken(start, end);
#endif // DO_PROPER_BENCHMARK
	}
	
	
	
	{
		EventHandler handler;
		typedef wink::event_queue<int> event_queue;
		event_queue sender;
		sender.connect(&handler, &EventHandler::queueHandleEvent);
		
		std::cout << "Using event_queue<int> to handle events:\n";
		
		
#ifdef DO_PROPER_BENCHMARK
		start = GetTimeNow();
#endif // DO_PROPER_BENCHMARK
		
		sender.reserve(numbersToSend.size()); // reserve some space
		sender.push(numbersToSend);
		
		for(int i = 0; i < AMOUNT_OF_TIMES_TO_SEND_EVENT; ++i)
		{
			// emit the event
			// (without clearing data)
			sender.cemit();
		}
		
#ifdef DO_PROPER_BENCHMARK
		end = GetTimeNow();
		
		// print the time taken
		printTimeTaken(start, end);
#endif // DO_PROPER_BENCHMARK
	}
	return 0;
}
