#pragma once

#include "../lib/joanrieu/signal11/signal11.h"

#include "../../benchmark.hpp"

class Jos
{
    SlotScope reg;

    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = signal11<Rng&>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        auto con = subject.connect(std::bind(&Foo::handler, &foo, std::placeholders::_1));

        // Automatically disconnect when the foo instance is destroyed
        // Benchmarks require connection management
        // Note that con must be captured by value because it is local
        foo.reg = make_slot_scope([con, &subject](void*) { subject.disconnect(con); });
    }
    template <typename Subject>
    static void emit_method(Subject& subject, Rng& rng)
    {
        subject(rng);
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

    static constexpr const char* C_LIB_NAME = "joanrieu signal11";
    static constexpr const char* C_LIB_SOURCE_URL = "https://github.com/joanrieu/signal11";
    static constexpr const char* C_LIB_FILE = "benchmark_jos";
    static constexpr const char* C_LIB_IS_HEADER_ONLY = "X";
    static constexpr const char* C_LIB_DATA_STRUCTURE = "std::list";
    static constexpr const char* C_LIB_IS_THREAD_SAFE = "-";

    static constexpr const std::size_t C_LIB_SIZEOF_SIGNAL = sizeof(Signal);
};
