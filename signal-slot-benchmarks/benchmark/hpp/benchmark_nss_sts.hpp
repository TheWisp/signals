#pragma once

#include "../lib/NoAvailableAlias/nano-signal-slot/nano_signal_slot.hpp"
#include "../lib/NoAvailableAlias/nano-signal-slot/nano_mutex.hpp"

#include "../../benchmark.hpp"

class Nss_sts : public Nano::Observer<Nano::ST_Policy_Safe>
{
    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    public:

    using Signal = Nano::Signal<void(Rng&), Nano::ST_Policy_Safe>;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        subject.template connect<&Foo::handler>(foo);
    }
    template <typename Subject>
    static void emit_method(Subject& subject, Rng& rng)
    {
        subject.fire(rng);
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

    static constexpr const char* C_LIB_NAME = "nano-signal-slot sts";
    static constexpr const char* C_LIB_SOURCE_URL = "https://github.com/NoAvailableAlias/nano-signal-slot";
    static constexpr const char* C_LIB_FILE = "benchmark_nss_sts";
    static constexpr const char* C_LIB_IS_HEADER_ONLY = "X";
    static constexpr const char* C_LIB_DATA_STRUCTURE = "std::vector";
    static constexpr const char* C_LIB_IS_THREAD_SAFE = "-";

    static constexpr const std::size_t C_LIB_SIZEOF_SIGNAL = sizeof(Signal);
};
