#Signals

## Why Yet Another Signal-Slot Library?

### Optimized for Low-Latency Applications
Such as video games.

High overhead using some previous libraries

### Explore New Designs in Modern C++
`template<auto>`

## Design Choices

### Direct (Blocking) Calls
In game systems, the logic flow often consists of many fast and weakly ordered function calls. 
No thread safety overhead

### Optimized for Emission
Latency is the bottleneck.

### O(1) Connection and Disconnection
In a dynamic world, receivers (slots) are often frequently created and destroyed. A linear search removal algorithm can easily become the performance bottleneck, especially when a high number of slots all disconnect at the same time. Removing by swapping with the end mitigates the problem, but the overall time spent removing N slots with a linear search would still be O(N^2). In this library, a slot is removed by marking its index unused, which then gets skipped and cleaned up in the next emission. Benchmarks have shown that the overhead is dominated by memory accessing (cache misses), rather than checking for null (pipeline stalling).

### Safe Recursion and Modification While Iterating
Just like direct function calls, recursions can naturally emerge from complex and dynamic behaviors. Furthermore, the signals and slots may be side-effected by their own results!

## Benchmark

Xeon E3-1275 V2 @ 3.90 GHz 16.0 GB RAM

Signal vs virtual (SAME class, SEQUENTIAL objects)
Signal (member func): 303 us
Signal (lambda): 335 us
Virtual call: 482 us
Signal (member func) takes 62% time of virtual function calls
Signal (lambda) takes 69% time of virtual function calls


Signal vs virtual (SAME class, RANDOM objects)
Signal (member func): 307 us
Signal (lambda): 354 us
Virtual call: 1336 us
Signal (member func) takes 22% time of virtual function calls
Signal (lambda) takes 26% time of virtual function calls


Signal vs virtual (RANDOM classes, SEQUENTIAL objects)
Signal (member func): 990 us
Signal (lambda): 995 us
Virtual call: 1288 us
Signal (member func) takes 76% time of virtual function calls
Signal (lambda) takes 77% time of virtual function calls


Signal vs virtual (RANDOM classes, RANDOM objects)
Signal (member func): 998 us
Signal (lambda): 1009 us
Virtual call: 1795 us
Signal (member func) takes 55% time of virtual function calls
Signal (lambda) takes 56% time of virtual function calls


