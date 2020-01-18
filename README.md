# Why yet another signal-slot library?

## Optimized for low-latency
Video games

High overhead using some previous libraries

## Explore new designs in modern C++
`template<auto>`

# Design choices

## Direct (blocking) calls
In game systems, the logic flow often consists of many fast and weakly ordered function calls. 
No thread safety overhead

## Optimized for emission
Latency is the bottleneck.

## Fast connection and disconnection
In a dynamic world such as video games, receivers (slots) can be frequently created and destroyed.

## Safe recursion and modification while iterating
Just like direct function calls, recursions can naturally emerge from complex and dynamic behaviors. Furthermore, the signals and slots may be side-effected by their own results!
