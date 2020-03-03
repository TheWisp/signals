signals-cpp [![Build Status](https://travis-ci.org/Kosta-Github/signals-cpp.png)](https://travis-ci.org/Kosta-Github/signals-cpp) [![Build status AppVeyor](https://ci.appveyor.com/api/projects/status/e7s580d8k277b8fn/branch/master?svg=true)](https://ci.appveyor.com/project/Kosta-Github/signals-cpp/branch/master)
===========
Provide a very simple header-only C++ interface for creating and managing signal-slot-connections in an easy and thread-safe manner.

misc
====
This library is header-only and uses some new C++11 features (such as `std::mutex`, `std::atomic`, lambda-expressions, and ...) to provide an easy-to-use interface and being thread-safe at the same time. There are several important advantages over the approach used by the Qt signal-and-slot implementation:
- no need for an additional build step (i.e., `MOC` compiler),
- no special macros and language extensions required (i.e., `Q_EMIT`, `Q_SLOTS`, `Q_OBJECT`, ...),
- not intrusive, since no inheritance from a certain base class is required (i.e., `QObject`),
- checking whether the signal interface matches that of the target slot happens during compile-time,
- no need for a meta-type system, and
- free-standing functions and lambda-expressions can also be used as a target slots.


sample usage
============
Expose a `signal` `valueChanged` with a single `int` parameter in a class `A` as a public member:
```
class A {
public:
    ...
	void setValue(int v);
    ...
public:
    sigs::signal<void(int)> valueChanged;
	...
};
```
The `signal` can be emitted or fired by calling its `fire()` method passing the actual `int` value as argument:
```
void A::setValue(int v) {
    ...
	valueChanged.fire(v);
}
```
To connect this `signal` to a method `onValueChanged()` of another class `B` add a `connections` object to class `B` (ideally as the *last* member):
```
class B {
public:
    B(A* a);
    ...
	void onValueChanged(int v);
    ...
private:
    ...
    sigs::connections m_conns; // should be the last member
};
```
To establish the actual `connection` (e.g., in the constructor of class `B`), call the `connect()` method of the `m_conns` member:
```
B::B(A* a) {
    ...
    m_conns.connect(a->valueChanged, this, &onValueChanged);
	...
}
```
You can also `connect` easily to a C++11 lambda expression like this:
```
B::B(A* a) {
    ...
    m_conns.connect(a->valueChanged, [](int v) {
	    std::cout << "signal 'valueChanged' triggered for: " << v << std::endl;
	});
	...
}
```
The `connections` get automatically disconnected on destruction of either object `a` or `b`, which ensures that no *dangling connections* exist.

external dependencies
=====================
- [cute](https://github.com/Kosta-Github/cute): only for unit tests
- [cmake](http://cmake.org): build system; only for the unit tests
