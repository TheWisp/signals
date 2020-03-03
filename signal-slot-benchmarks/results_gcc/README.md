
# GCC (Linux) HELP NEEDED

~~**_Higher score is better._** _[2, 64] / [(sample size / count)](../benchmark.hpp#L11)._~~

_Running the benchmark outputs all the necessary markdown used to update these results.
All that is required is a proper GCC environment! (wait does the cmake file even work?)_

### Performance of Thread Safe Libraries

| Library | threaded | combined | emission | connect | destruct | construct | score |
|---------|----------|----------|----------|---------|----------|-----------|-------|
| ! Pal Sigslot | 1166 | 5440 | 70240 | 10716 | 12937 | 163314 | 263813 |
| * fr00b0 nod | 994 | 3847 | 73628 | 7021 | 9962 | 164957 | 260409 |
| ! Kosta signals-cpp | 12 | 911 | 27394 | 1279 | 7448 | 192119 | 229164 |
| ! lsignal | 883 | 1932 | 72456 | 3114 | 5665 | 142482 | 226531 |
| * neosigslot | 410 | 1687 | 13345 | 3871 | 4309 | 90011 | 113634 |
| * Boost Signals2 | 56 | 1572 | 17062 | 3668 | 4462 | 15082 | 41902 |

### Performance of Thread Unsafe Libraries

| Library | threaded | combined | emission | connect | destruct | construct | score |
|---------|----------|----------|----------|---------|----------|-----------|-------|
| jeffomatic jl_signal | - | 14402 | 83513 | 66056 | 20536 | 124488 | 308996 |
| Pal Sigslot ST | - | 6131 | 79574 | 13636 | 12923 | 174087 | 286351 |
| mwthinker Signal | - | 3181 | 81663 | 5471 | 8271 | 174375 | 272962 |
| Yassi | - | 2925 | 79241 | 4812 | 8789 | 168052 | 263818 |
| Wink-Signals | - | 7271 | 82834 | 15519 | 14730 | 141276 | 261631 |
| pbhogan Signals | - | 4803 | 52850 | 9975 | 10074 | 179176 | 256877 |
| amc522 Signal11 | - | 3267 | 79348 | 5593 | 9152 | 158937 | 256297 |
| joanrieu signal11 | - | 3978 | 15947 | 10148 | 12002 | 187540 | 229615 |
| supergrover sigslot | - | 1330 | 79462 | 4164 | 2092 | 138097 | 225145 |
| EvilTwin Observer | - | 1717 | 47223 | 2741 | 5406 | 141403 | 198489 |

___
_Size results are the size of object files from [architecture] release build with Gcc 6.3._

### Metrics of Thread Safe Libraries

| Library | Build Size | Signal Size | Header Only | Data Structure | Thread Safe |
| ------- |:----------:|:-----------:|:-----------:| -------------- |:-----------:|
| [Pal Sigslot](https://github.com/palacaze/sigslot) | 132 kb | ? | X | singly linked list | ! |
| [Kosta-Github signals-cpp](https://github.com/Kosta-Github/signals-cpp) | 140 kb | ? | X | std::vector | ! |
| [fr00b0 nod](https://github.com/fr00b0/nod) | 148 kb | ? | X | std::vector | X |
| [cpp11nullptr lsignal](https://github.com/cpp11nullptr/lsignal) | 155 kb | ? | X | **std::list | ! |
| [neosigslot](http://www.i42.co.uk/stuff/neosigslot.htm) | 224 kb | ? | - | **std::map | X |
| [Boost Signals2](http://www.boost.org/doc/libs/1_58_0/doc/html/signals2.html) | 298 kb | ? | ? | ? | X |

### Metrics of Thread Unsafe Libraries

| Library | Build Size | Signal Size | Header Only | Data Structure | Thread Safe |
| ------- |:----------:|:-----------:|:-----------:| -------------- |:-----------:|
| [jeffomatic jl_signal](https://github.com/jeffomatic/jl_signal) | 37 kb | ? | - | doubly linked list | - |
| [Wink-Signals](https://github.com/miguelmartin75/Wink-Signals) | 40 kb | ? | X | std::vector | - |
| [supergrover sigslot](https://github.com/supergrover/sigslot) | 42 kb | ? | - | std::list | - |
| [pbhogan Signals](https://github.com/pbhogan/Signals) | 43 kb | ? | X | std::set | - |
| [joanrieu signal11](https://github.com/joanrieu/signal11) | 48 kb | ? | X | std::list | - |
| [Yassi](http://www.codeproject.com/Articles/867044/Yassi-Yet-Another-Signal-Slot-Implementation) | 51 kb | ? | X | std::vector | - |
| [mwthinker Signal](https://github.com/mwthinker/Signal) | 54 kb | ? | - | std::list | - |
| [Pal Sigslot ST](https://github.com/palacaze/sigslot) | 59 kb | ? | X | singly linked list | - |
| [EvilTwin Observer](http://eviltwingames.com/blog/the-observer-pattern-revisited/) | 69 kb | ? | X | std::vector | - |
| [amc522 Signal11](https://github.com/amc522/Signal11) | 72 kb | ? | X | std::vector | - |

| * | ** | ! | [] |
|:-:|:--:|:-:|:--:|
| _Supports thread-safety_ | _Uses other containers_ | **Has thread-safety issues** | Excluded from scoring |

Benchmark Algorithms
--------------------

_The individual benchmark algorithms are completely generic through the use of templates._

| Algorithm | Description |
| --------- | ----------- |
| [validation_assert](../benchmark.hpp#L19) | Make sure each signal implementation is functioning correctly. |
| [construction](../benchmark.hpp#L48) | Sample the default construction of N * N number of Signal instances. |
| [destruction](../benchmark.hpp#L66) | Sample the destruction of N * N number of Signal instances. |
| [connection](../benchmark.hpp#L85) | Sample Signal connections to N number of Foo instances. |
| [disconnect](../benchmark.hpp#L108) | Sample disconnecting N number of Foo instances from a single Signal. |
| [reconnect](../benchmark.hpp#L135) | Sample reconnecting N number of Foo instances to a global Signal. |
| [emit](../benchmark.hpp#L159) | Sample the duration of an N slot emission. |
| [all](../benchmark.hpp#L184) | Sample all previous benchmarks together in one combined benchmark. |
| [threaded](../benchmark.hpp#L208) | Same as the previous benchmark but is now threaded. |
<br/>
