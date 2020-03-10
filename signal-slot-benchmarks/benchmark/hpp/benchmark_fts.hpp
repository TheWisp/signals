#pragma once

#include "../lib/fteng/signals.hpp"

#include "../../benchmark.hpp"

class Fts
{
    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

public:

    using Signal = fteng::signal<void(Rng&)>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        subject.template connect<&Foo::handler>(&foo);
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
    static constexpr const char* C_LIB_NAME = "FTeng Signals";
    static constexpr const char* C_LIB_SOURCE_URL = "https://github.com/TheWisp/signals";
    static constexpr const char* C_LIB_FILE = "benchmark_fts";
    static constexpr const char* C_LIB_IS_HEADER_ONLY = "X";
    static constexpr const char* C_LIB_DATA_STRUCTURE = "std::vector";
    static constexpr const char* C_LIB_IS_THREAD_SAFE = "-";

    static constexpr const std::size_t C_LIB_SIZEOF_SIGNAL = sizeof(Signal);
};
