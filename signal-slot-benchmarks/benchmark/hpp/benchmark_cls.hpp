#pragma once

#include "../lib/cpp11nullptr/lsignal/lsignal.h"

#include "../../benchmark.hpp"

class Cls
{
    SlotScope reg;

    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:
    
    // Had to add namespace, always namespace
    using Signal = lsignal::signal<void(Rng&)>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        auto reg = subject.connect(&foo, &Foo::handler);
        // Automatically disconnect when the foo instance is destroyed
        // Benchmarks require connection management
        foo.reg = make_slot_scope([reg, &subject](void*) { subject.disconnect(reg); });

        // does not compile due to "cannot convert this pointer from const lsignal::connection to lsignal::connection&"
        //foo.reg = make_slot_scope([reg, &subject](void*) { reg.disconnect(); });
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
    static double threaded(std::size_t, std::size_t);

    // The following is used for report outputs
    static constexpr const char* C_LIB_NAME = "* cpp11nullptr lsignal";
    static constexpr const char* C_LIB_SOURCE_URL = "https://github.com/cpp11nullptr/lsignal";
    static constexpr const char* C_LIB_FILE = "benchmark_cls";
    static constexpr const char* C_LIB_IS_HEADER_ONLY = "X";
    static constexpr const char* C_LIB_DATA_STRUCTURE = "**std::list";
    static constexpr const char* C_LIB_IS_THREAD_SAFE = "X";

    static constexpr const std::size_t C_LIB_SIZEOF_SIGNAL = sizeof(Signal);
};
