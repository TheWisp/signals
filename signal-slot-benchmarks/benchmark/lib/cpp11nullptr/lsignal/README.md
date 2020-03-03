# lsignal: C++ signal/slot system.

lsignal (or lightweight signal) is a very little and fast C++ thread-safe implementation of signal and
slot system which is based on modern C++11 code.

### Requirements

C++ compiler with support C++11.

### How to use

Include `lsignal.h` in your project.

### Essential classes

##### signal

This is a template class which holds callbacks and can emit signals with certain
arguments. See examples of declarations:

| Declaration                         | Description                                                            |
|------------------------------------ |------------------------------------------------------------------------|
| `lsignal::signal<void()> s;`        | Signal without parameters, return type - _void_                        |
| `lsignal::signal<int(int,int)> t;`  | Signal with two parameters, return type - _int_                        |
| `lsignal::signal<std::string()> u;` | Signal without parameters, return type - _std::string_                 |

You can connect to signal any callback which looks like callable object but be aware than
signature of callback must be equal signature of corresponding signal:

| Callback                            | Description                                                            |
|-------------------------------------|------------------------------------------------------------------------|
| `s.connect(foo);`                   | _foo_ is a common function                                             |
| `s.connect(bar);`                   | _bar_ is a lambda function                                             |
| `s.connect(baz);`                   | _baz_ is a class with operator()                                       |
| `s.connect(&qx, &qux::func);`       | _qx_ is a instance of class qux                                        |

Result of this function is a instance of class `connection`.

When signal is emitted return value will be the result of executing last connected callback.
If you want to receive all results of callbacks you should pass aggregate function as last parameter:

```cpp
lsignal::signal<int(int,int)> s;
...
auto agg = [](const std::vector<int>& v) -> int { ... };
s(2, 3, agg);
```

##### connection

`connection` contains link between signal and callback. Available next operations:

| Method                            | Description                                                            |
|-----------------------------------|------------------------------------------------------------------------|
| `is_locked`                       | Check if connection is locked                                          |
| `set_lock`                        | If connection is locked then callback won't be called                  |
| `disconnect`                      | Remove callback from signal                                            |

Also you can pass `connection` directly to `signal::disconnect` for disconnecting this connection.

##### slot

This class similar to `connection` but is used for owhership policy. Look example:

```cpp
class foo : public lsignal::slot
{
    ...
};
...
foo f;

// disconnect when f was destroyed
s.connect([](){ ... }, &f);
```

### Performance

Synthetic test (one or more empty callbacks) showed that calling `lsignal` from two
to five times faster than calling `boost::signal2` which was created with dummy (empty) mutex.
