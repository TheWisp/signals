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
In a dynamic world, slots (receivers) are often frequently created and destroyed. A linear search removal algorithm can easily become the performance bottleneck, especially when a large number of slots all disconnect at the same time. Removing by swapping with the end mitigates the problem, but the overall time spent removing N slots with a linear search would still be O(N^2). In this library, a slot is removed by marking its index unused, which then gets skipped and cleaned up in the next emission. Benchmarks have shown that the overhead is dominated by memory accessing (cache misses), rather than checking for null (pipeline stalling).

### Safe Recursion and Modification While Iterating
Just like direct function calls, recursions can naturally emerge from complex and dynamic behaviors. Furthermore, the signals and slots may be side-effected by their own results!

## Usage
Simply include the single header, `signals.hpp`.
A C++17 compliant compiler is necessary.
Give it a try on [Godbolt](https://godbolt.org/z/TKEaZ9)!

### Basics
The following example demonstrates how to define, connect and emit a signal.

```cpp
// A function callback
void on_update(float delta) { }

// A member function callback
class my_class{
  void on_update(float delta) { }
};

int main()
{
  // A signal specifying its signature in the template parameter
  fteng::signal<void(float delta)> update;

  // Connects to a function callback
  update.connect(on_update);

  // Connects to an object's member function
  my_class* my_obj = new my_class;
  update.connect<&my_class::on_update>(my_obj);

  // Connects to a lambda callback
  update.connect([](float delta) { });

  // Connects to a generic lambda callback
  update.connect([](auto&&... as) { });

  // Emits the signal
  update(3.14f);

  delete my_obj;
}
```

Signals automatically disconnect from their slots (receivers) upon destruction.
```cpp
class button{
  public: fteng::signal<void(button& btn, bool down)> pressed;
};

class my_special_frame {
  std::vector<button> buttons;

  my_special_frame() {
    buttons.emplace_back();
    buttons.back().pressed.connect<&my_special_frame::on_button_pressed>(this);
  }

  void on_button_pressed(button& btn, bool down) {
    /* ... */
  }
};
```

### Connection Management
Slots don't automatically disconnect from the signal when they go out of scope. 
This is due to the non-intrusive design and the "pay only for what you use" principle.

To help automatically disconnect the slot, the `connect()` method returns an unmanaged (raw) connection, which may be converted to a `fteng::connection` representing the unique ownership.
It is recommended to save this connection into the slot's structure in order to automatically disconnect from the signal in an RAII fashion.

The following design would automatically disconnect the object from the signal when it is deleted.

```cpp
class game { /*...*/ };
fteng::signal<void(const game& instance)> game_created;

class subsystem
{
  //Connects a signal with a lambda capturing 'this'
  fteng::connection on_game_created = game_created.connect([this](const game& instance)
  {
    std::cout << "Game is created!\n";
  });
};

int main()
{
  subsystem* sys1 = new subsystem;

  game game_instance;
  game_created(game_instance); // Notifies each subsystem

  delete sys1; // Automatically disconnects from the signal

  game game_instance2;
  game_created(game_instance2); // Notifies each subsystem. Should not crash.
}
```

Alternatively, you may use a member function for callback.

```cpp
class subsystem
{
  //Connects a signal with a member function
  fteng::connection on_game_created = game_created.connect<&subsystem::on_game_created_method>(this);

  void on_game_created_method(const game& instance)
  {
    std::cout << "Game is created, now we can create other systems!\n";
  };
};
```

A few important notes about the `connection` object:
- `connection` is default-constructible, moveable but not copyable.
- Destroying the `connection` object would automatically disconnect the associated signal and slot.
- If you know the slot outlives the signal, it's fine to connect them without saving the connection object. There won't be any memory leak.
- If the signal can outlive the slots, store the `connection` in the slot's structure so that it disconnects the signal automatically.

### Connecting / Disconnecting Slots from Callback
Sometimes during the callback, we might want to disconnect the slot from the signal. There are also cases where we want to create or destroy other objects, who just happen to observe the same signal that triggered the callback. The following examples demonstrates how these usage are supported by the library.
```cpp
fteng::signal<void(entity eid)> entity_created;

class A
{
  std::unique_ptr<B> b;

  fteng::connection on_entity_created = entity_created.connect([this](entity eid)
  {
    // Creates a 'B' which also connects to the signal.
    // It's fine to connect more objects to the signal during the callback, 
    // With a caveat that they won't be notified this time (but next time).
    b = std::make_unique<B>(); 
  });
};

class B
{
  // C is some class that also listens to entity_created
  std::vector<C*> cs;

  fteng::connection on_entity_created = entity_created.connect([this](entity eid)
  {
    /* ... */
    if (eid == some_known_eid){

      // Imagine this operation automatically disconnects all C objects from entity_created
      // It's fine to disconnect any object from the signal during the callback, no matter if it's
      // the object being called back or any other object. The disconnected objects are skipped over.
      for (C* c : cs) 
        delete c;

      // Also fine
      on_entity_created.disconnect();

      // Also fine - Don't do this in modern C++ though ...
      delete this;
    }
  });
};
```

### Blocking a Connection
A connection can be temporarily disabled with `block()`, so that it won't be notified by the signal until it has been `unblock()` ed again.
```cpp
fteng::signal<void()> sig;

class Foo
{
  fteng::connection conn = sig.connect([this](){
    conn.block();
    sig(); // Now this won't cause an infinite recursion.
    conn.unblock();
  });
};
```

## Benchmark

Xeon E3-1275 V2 @ 3.90 GHz 16.0 GB RAM

|                                  | Signal (member func) | Signal (lambda) | Virtual Call | Sig(mem func) % of Virtual | Sig(lambda) % of Virtual |
| -------------------------------- | -------------------- | --------------- | ------------ | -------------------------- | ------------------------ |
| SAME class, SEQUENTIAL objects   | 303 us               | 335 us          | 482 us       | 62%                        | 69%                      |
| SAME class, RANDOM objects       | 307 us               | 354 us          | 1336 us      | 22%                        | 26%                      |
| RANDOM class, SEQUENTIAL objects | 990 us               | 995 us          | 1288 us      | 76%                        | 77%                      |
| RANDOM class, RANDOM objects     | 998 us               | 1009 us         | 1795 us      | 55%                        | 56%                      |
