
# MSVC (Windows)

**_Higher score is better._** _[2, 64] / [(sample size / count)](../benchmark.hpp#L11)._

### Performance of Thread Safe Libraries

| Library | [constr] | [destr] | conn | disconn | reconn | emit | all | threaded | score |
|---------|----------|---------|------|---------|--------|------|----------|----------|-------|
| * nano-signal-slot ts | 36517 | 12664 | 10350 | 17276 | 13779 | 134282 | 7995 | 575 | 184257 |
| * palacaze sigslot | 3942 | 2929 | 9762 | 12415 | 14554 | 113508 | 6389 | 499 | 157127 |
| * Montellese cpp-signal | 17298 | 5237 | 9736 | 9762 | 9691 | 121895 | 4565 | 488 | 156138 |
| * cpp11nullptr lsignal | 2017 | 2314 | 4491 | 7966 | 4440 | 119715 | 2790 | 380 | 139783 |
| * fr00b0 nod | 16840 | 13955 | 10996 | 20589 | 25688 | 53478 | 9496 | 219 | 120466 |
| * CppFakeIt FastSignals | 3307 | 4200 | 7467 | 16102 | 17234 | 44998 | 6968 | 115 | 92884 |
| * Kosta signals-cpp | 18152 | 5581 | 3553 | 43849 | 3016 | 36380 | 2568 | 297 | 89662 |
| * nano-signal-slot tss | 3522 | 2764 | 8842 | 7390 | 12951 | 23562 | 3545 | 345 | 56635 |
| * Boost Signals2 | 316 | 567 | 3586 | 26174 | 2772 | 21597 | 90 | 9 | 54228 |
| * dacap observable | 12873 | 1295 | 9764 | 9063 | 10957 | 16166 | 3852 | 11 | 49814 |
| * neolib event | 16732 | 598 | 2244 | 4089 | 2598 | 5885 | 1197 | 89 | 16102 |

### Performance of Thread Unsafe Libraries

| Library | [constr] | [destr] | conn | disconn | reconn | emit | all | threaded | score |
|---------|----------|---------|------|---------|--------|------|----------|----------|-------|
| jeffomatic jl_signal | 29144 | 19613 | 108343 | 39417 | 107186 | 141612 | 23657 | 0 | 420215 |
| Wink-Signals | 48418 | 51159 | 14583 | 23672 | 28813 | 141754 | 12470 | 0 | 221292 |
| Nuclex Events | 58255 | 44933 | 21625 | 21735 | 21196 | 138949 | 10217 | 0 | 213722 |
| Ansoulom cpp-observe | 48310 | 34152 | 9969 | 22903 | 14509 | 141534 | 8510 | 0 | 197424 |
| nano-signal-slot st | 48374 | 27898 | 11334 | 19902 | 18032 | 137019 | 9196 | 0 | 195482 |
| palacaze sigslot | 35430 | 19137 | 11055 | 15466 | 20265 | 140358 | 7994 | 0 | 195139 |
| Yassi | 48019 | 34464 | 10549 | 13635 | 20998 | 141297 | 8139 | 0 | 194617 |
| Montellese cpp-signal | 58313 | 17895 | 13600 | 15222 | 13904 | 140828 | 6910 | 0 | 190464 |
| vdksoft signals | 3509 | 5900 | 13637 | 19121 | 14277 | 134205 | 7609 | 0 | 188850 |
| mwthinker Signal | 4496 | 6065 | 10041 | 12274 | 9708 | 138232 | 5399 | 0 | 175654 |
| pbhogan Signals | 2930 | 5571 | 10435 | 10748 | 7988 | 141883 | 4299 | 0 | 175353 |
| fr00b0 nod | 27096 | 19281 | 15995 | 32863 | 48079 | 60938 | 15544 | 0 | 173418 |
| SimpleSignal | 3840 | 6139 | 8641 | 9218 | 8556 | 141654 | 4170 | 0 | 172239 |
| amc522 Signal11 | 20992 | 32419 | 8681 | 14266 | 9123 | 130925 | 5656 | 0 | 168651 |
| joanrieu signal11 | 1649 | 2925 | 11353 | 16915 | 11296 | 116547 | 6558 | 0 | 162669 |
| supergrover sigslot | 2102 | 3207 | 4118 | 4998 | 4109 | 142148 | 2067 | 0 | 157440 |
| nano-signal-slot sts | 48477 | 27921 | 11331 | 19930 | 18062 | 89583 | 8595 | 0 | 147500 |
| EvilTwin Observer | 3982 | 4260 | 4601 | 8503 | 5549 | 62978 | 2992 | 0 | 84623 |
| copperspice cs_signal | 7411 | 2122 | 2699 | 3090 | 2966 | 13366 | 1333 | 0 | 23453 |
| neolib event | 16851 | 606 | 2304 | 4619 | 2776 | 6002 | 1289 | 0 | 16989 |

___
_Size results are the size of object files from x64 release build with Visual Studio 17._

### Metrics of Thread Safe Libraries

| Library | Build Size | Signal Size | Header Only | Data Structure | Thread Safe |
| ------- |:----------:|:-----------:|:-----------:| -------------- |:-----------:|
| [* nano-signal-slot ts](https://github.com/NoAvailableAlias/nano-signal-slot) | 842 kb | 32 b | X | std::vector | X |
| [* CppFakeIt FastSignals](https://github.com/CppFakeIt/FastSignals) | 854 kb | 16 b | - | std::vector | X |
| [* dacap observable](https://github.com/dacap/observable) | 854 kb | 184 b | - | std::vector | X |
| [* Montellese cpp-signal](https://github.com/Montellese/cpp-signal) | 856 kb | 96 b | X | std::forward_list | X |
| [* nano-signal-slot tss](https://github.com/NoAvailableAlias/nano-signal-slot) | 900 kb | 48 b | X | std::vector | X |
| [* palacaze sigslot](https://github.com/palacaze/sigslot) | 921 kb | 104 b | X | std::vector | X |
| [* fr00b0 nod](https://github.com/fr00b0/nod) | 924 kb | 144 b | X | std::vector | X |
| [* Kosta signals-cpp](https://github.com/Kosta-Github/signals-cpp) | 931 kb | 96 b | X | std::vector | X |
| [* cpp11nullptr lsignal](https://github.com/cpp11nullptr/lsignal) | 1011 kb | 128 b | X | **std::list | X |
| [* Boost Signals2](http://www.boost.org/doc/libs/1_58_0/doc/html/signals2.html) | 2179 kb | 32 b | - | ? | X |
| [* neolib event](https://github.com/i42output/neolib) | 2638 kb | 136 b | - | **std::unordered_map | X |

### Metrics of Thread Unsafe Libraries

| Library | Build Size | Signal Size | Header Only | Data Structure | Thread Safe |
| ------- |:----------:|:-----------:|:-----------:| -------------- |:-----------:|
| [jeffomatic jl_signal](https://github.com/jeffomatic/jl_signal) | 199 kb | 40 b | - | doubly linked list | - |
| [Montellese cpp-signal](https://github.com/Montellese/cpp-signal) | 226 kb | 16 b | X | std::forward_list | - |
| [nano-signal-slot st](https://github.com/NoAvailableAlias/nano-signal-slot) | 230 kb | 24 b | X | std::vector | - |
| [Wink-Signals](https://github.com/miguelmartin75/Wink-Signals) | 245 kb | 24 b | X | std::vector | - |
| [nano-signal-slot sts](https://github.com/NoAvailableAlias/nano-signal-slot) | 247 kb | 24 b | X | std::vector | - |
| [Nuclex Events](http://blog.nuclex-games.com/2019/10/nuclex-signal-slot-benchmarks/) | 251 kb | 40 b | X | dynamic array | - |
| [vdksoft signals](https://github.com/vdksoft/signals) | 257 kb | 16 b | - | singly linked list | * |
| [supergrover sigslot](https://github.com/supergrover/sigslot) | 266 kb | 40 b | - | std::list | - |
| [mwthinker Signal](https://github.com/mwthinker/Signal) | 280 kb | 32 b | - | std::list | - |
| [pbhogan Signals](https://github.com/pbhogan/Signals) | 288 kb | 16 b | X | std::set | - |
| [palacaze sigslot](https://github.com/palacaze/sigslot) | 292 kb | 48 b | X | std::vector | - |
| [joanrieu signal11](https://github.com/joanrieu/signal11) | 304 kb | 32 b | X | std::list | - |
| [Ansoulom cpp-observe](https://github.com/Ansoulom/cpp-observe) | 307 kb | 24 b | X | std::vector | - |
| [Yassi](http://www.codeproject.com/Articles/867044/Yassi-Yet-Another-Signal-Slot-Implementation) | 312 kb | 24 b | X | std::vector | - |
| [SimpleSignal](https://github.com/larspensjo/SimpleSignal) | 325 kb | 16 b | X | std::vector | - |
| [EvilTwin Observer](http://eviltwingames.com/blog/the-observer-pattern-revisited/) | 327 kb | 40 b | X | std::vector | - |
| [fr00b0 nod](https://github.com/fr00b0/nod) | 342 kb | 72 b | X | std::vector | - |
| [amc522 Signal11](https://github.com/amc522/Signal11) | 351 kb | 16 b | X | std::vector | - |
| [copperspice cs_signal](https://github.com/copperspice/cs_signal) | 463 kb | 128 b | - | **RCU List | DIY |
| [neolib event](https://github.com/i42output/neolib) | 2084 kb | 136 b | - | **std::unordered_map | - |

| * | ** | [] |
|:-:|:--:|:--:|
| _Supports thread-safety_ | _Uses additional containers_ | Excluded from scoring |

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
