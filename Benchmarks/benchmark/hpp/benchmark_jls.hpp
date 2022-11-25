#pragma once

#include "../lib/jeffomatic/jl_signal/src/Signal.h"

#include "../../benchmark.hpp"

class Jls : public jl::SignalObserver
{
    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = jl::Signal<void(Rng&)>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        subject.Connect(&foo, &Foo::handler);
    }
    template <typename Subject>
    static void emit_method(Subject& subject, Rng& rng)
    {
        subject.Emit(rng);
    }

    // Used for switching policies at runtime
    static void initialize();

    static void validate_assert(std::size_t);
    static double construction(std::size_t, std::size_t);
    static double destruction(std::size_t, std::size_t);
    static double connection(std::size_t, std::size_t);
    static double disconnect(std::size_t, std::size_t);
    static double reconnect(std::size_t, std::size_t);
    static double emission(std::size_t, std::size_t);
    static double combined(std::size_t, std::size_t);

    // NOT IMPLEMENTED FOR THIS LIB
    static double threaded(std::size_t, std::size_t);

    static constexpr const char* C_LIB_NAME = "jeffomatic jl_signal";
    static constexpr const char* C_LIB_SOURCE_URL = "https://github.com/jeffomatic/jl_signal";
    static constexpr const char* C_LIB_FILE = "benchmark_jls";
    static constexpr const char* C_LIB_IS_HEADER_ONLY = "-";
    static constexpr const char* C_LIB_DATA_STRUCTURE = "doubly linked list";
    static constexpr const char* C_LIB_IS_THREAD_SAFE = "-";

    static constexpr const std::size_t C_LIB_SIZEOF_SIGNAL = sizeof(Signal);
};
