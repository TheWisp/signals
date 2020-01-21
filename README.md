# Signals

## Why Yet Another Signal-Slot Library?
This library is optimized for video games (and probably other low-latency applications as well). Interestingly, even though the observer pattern is generally useful, it has never been standardized in C++, which leads to the never-ending attempts at improvements by curious people. I view it as a good thing, because if `boost::signals2` was standardized, [it would have been a disaster for real-time applications](https://stackoverflow.com/questions/22416860/is-boostsignals2-overkill-for-simple-applications).

There are many similar libraries - such as [jl_signal](http://hoyvinglavin.com/2012/08/06/jl_signal/), [nuclex signal/slots](http://blog.nuclex-games.com/2019/10/nuclex-signal-slot-benchmarks/) and [several dozens more](https://github.com/NoAvailableAlias/signal-slot-benchmarks). My work is based on a previous [research](https://github.com/TheWisp/ImpossiblyFastEventCPP17) which focused on the syntax and performance improvements brought by a C++17 feature - `template<auto>`. This library is a mixture of modern C++ exploration, system programming and data-structure design. It aims to become feature-complete like `boost::signals`, yet extremely light-weight - both run time and memory footprint - in order to replace _interface_ or `std::function` based callbacks.

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

## Benchmark

Xeon E3-1275 V2 @ 3.90 GHz 16.0 GB RAM

|                                  | Signal (member func) | Signal (lambda) | Virtual Call | Sig(mem func) % of Virtual | Sig(lambda) % of Virtual |
| -------------------------------- | -------------------- | --------------- | ------------ | -------------------------- | ------------------------ |
| SAME class, SEQUENTIAL objects   | 303 us               | 335 us          | 482 us       | 62%                        | 69%                      |
| SAME class, RANDOM objects       | 307 us               | 354 us          | 1336 us      | 22%                        | 26%                      |
| RANDOM class, SEQUENTIAL objects | 990 us               | 995 us          | 1288 us      | 76%                        | 77%                      |
| RANDOM class, RANDOM objects     | 998 us               | 1009 us         | 1795 us      | 55%                        | 56%                      |
