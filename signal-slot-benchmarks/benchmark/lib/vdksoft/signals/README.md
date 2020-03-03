C++ Signals & Slots (vdk-signals)
=================================

## Introduction

Signals and slots is a mechanism used for communication between different parts of some application. The key concept is allowing objects of any kind to communicate with one another while keeping them as much decoupled as possible. Signals are emitted by an object when its internal state has changed in some way that might be interesting to other objects. Slots are called when signals connected to them are emitted. Slots can be used to receive signals, but they are also normal C++ functions and can be called directly.

The signals and slots mechanism is type safe. The signal type must match the signature of the receiving slot and the compiler helps to detect type mismatches generating compile-time errors. Each signal can be connected to any number of slots and each slot can be connected to any number of signals. It is even possible to connect a signal directly to another signal.

The signals and slots mechanism is especially convenient for Graphical User Interface programming, but can also be used pretty much everywhere. It enables a powerful and flexible component programming mechanism that makes code easy to design and reuse.

## Overview

**vdk-signals** is a one-header library, which implements a type-safe, thread-safe signals-slots system in C++. It is written entirely in pure standard C++ with high performance and efficiency in mind. **vdk-signals** relies on C++ Standard Library only, and does not require any pre-compilation steps, special tools, third party libraries or non-standard extensions. Everything you need is in one header, just put it in your project, #include and use. The interface of the library is designed to be intuitive, very simple, and with clean syntax.

**In order to use this library, compiler must be compatible with at least C++11 Standard.**

High speed is achieved through the use of modern C++ features and atomic variables to ensure thread-safety and fastest possible execution. Specialized synchronization mechanism makes signal emission lock-free and wait-free, avoiding any undesirable consequences introduced by locks.

The core of this library is **vdk::signal** template class. This class exposes all public methods and encapsulates a set of internal classes. An instance of **vdk::signal** contains a list of connections (connected slots). When a particular event occurs the signal is emitted and all connected slots are executed one after another.

Slots are normal C++ functions, which are called in response to a particular signal emission. Any callable target can be connected to some signal as a slot. It may be a free (global) function, static member function of some class (static method), member function of some class (method) with any qualifiers (like const, volatile), function object (functor), lambda or even another signal.

**vdk-signals** library supports auto-disconnection feature. A signal object can track connected slots through std::shared_ptr / std::weak_ptr and automatically disconnect destroyed slots, once such slots are detected. This tracking mechanism makes it possible to avoid needless and intrusive inheritance for tracked objects.

Special care has been taken regarding memory usage efficiency to ensure complete absence of memory leaks or memory fragmentation. You can choose whether you want to allocate all memory for the signal object as one memory block up-front, or allocate it as separate parts later when your program runs.

In some cases, when user is absolutely sure that the library will be used in single-threaded environment only, one could activate special library mode to enable signals optimized for single-threaded usage. For that purpose, **"VDK_SIGNALS_LITE"** macro should be defined before including the library header. This macro switches the library mode making it single-threaded, so that user can avoid any unnecessary overhead introduced by thread-safety and benefit from keeping every bit of performance. This single-threaded mode is also optimized for memory usage efficiency. Don’t pay for what you don’t use!

This library enables any object in your program work as an independent software component that can communicate with other unrelated components through signals and slots. This approach allows true decoupling, makes your design clearer, and increases reusability of your code.

## Features

1. Single-header implementation. Does not require any special compilation. Just put it in the project, #include and use;
2. Signal class is solid and opaque. There is no mess with helper classes or functions. Signal can be treated as built-in type;
3. Simple to use, with intuitive and clean syntax, but without any limitations of necessary functionality in the same time;
4. Thread-safe signals with excellent concurrency capabilities. C++11 atomics ensure fast work even in high-load situations;
5. Type-safe signals with compile-time checks. The compiler detects type mismatches generating compile-time errors;
6. Efficient memory usage. Memory can be allocated up-front in single memory block. Eliminates memory fragmentation;
7. Library relies on standard modern C++ only, without any non-standard extensions, meta-object compilers or other tools;
8. Signals support auto-disconnection feature. Expired / destroyed slots are detected and disconnected automatically;
9. Signals are able to connect any type of callable targets, including methods with qualifiers, lambdas and other signals;
10. Special single-threaded mode is available. Applications that work in single thread does not lose any bit of performance;
11. Exception safety. No thrown exception can leave a signal object in undetermined state. All exceptions are documented.



### **Full documentation, tutorial, performance comparison, and API reference can be found in the library package.**