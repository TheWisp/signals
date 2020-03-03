
# signal-slot-benchmarks

Comprehensive benchmarks for a majority of GitHub c++ signal slot implementations and others.
<br/>
**_Boost, as well as C++17 support, is required to build this project._**

Foreword
--------

These benchmarks do not accurately depict real world usage patterns seen in the wild.
<br/>
However, these benchmarks were designed to provide impartial results regardless of implementation.
<br/>
It is my goal to take what I've learned from these results and apply this knowledge into making nano-signal-slot better.

Tested Environments
-------------------

<div style="text-align:center; margin-bottom:1em;">click to view results</div>
<div style="text-align:center; display:flex;">
	<div style="flex:1;">
    	<a href="/results_msvc/README.md">
        	<button type="button" style="padding:2em; cursor:pointer;  font-size:2em; width:90%;">
            	MSVC (Windows)
            </button>
        </a>
	</div>
	<div style="flex:1;">
        <a href="/results_gcc/README.md">
            <button type="button" style="padding:2em; cursor:pointer; font-size:2em; width:90%;">
            	GCC (Linux)
            </button>
        </a>
	</div>
</div>

Maintainers
-----------

Generally, implementations that get added to this benchmark project aren't regularly updated by me unless asked to do so.
If you are a maintainer of a referenced library, I encourage you to fork this project and create pull requests to expedite updates.

Benchmark Algorithms
--------------------

_The individual benchmark algorithms are completely generic through the use of templates._

| Algorithm | Description |
| --------- | ----------- |
| [validation_assert](benchmark.hpp#L19) | Make sure each signal implementation is functioning correctly. |
| [construction](benchmark.hpp#L48) | Sample the default construction of N * N number of Signal instances. |
| [destruction](benchmark.hpp#L66) | Sample the destruction of N * N number of Signal instances. |
| [connection](benchmark.hpp#L85) | Sample Signal connections to N number of Foo instances. |
| [disconnect](benchmark.hpp#L108) | Sample disconnecting N number of Foo instances from a single Signal. |
| [reconnect](benchmark.hpp#L135) | Sample reconnecting N number of Foo instances to a global Signal. |
| [emit](benchmark.hpp#L159) | Sample the duration of an N slot emission. |
| [all](benchmark.hpp#L184) | Sample all previous benchmarks together in one combined benchmark. |
| [threaded](benchmark.hpp#L208) | Same as the previous benchmark but is now threaded. |
<br/>
