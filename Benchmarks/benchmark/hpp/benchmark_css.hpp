#pragma once

//#include <deque>

#include "../lib/copperspice/cs_signal/src/cs_signal.h"
#include "../lib/copperspice/cs_signal/src/cs_slot.h"

#include "../../benchmark.hpp"

class Css_Signal : public CsSignal::SignalBase
{
    public:

    SIGNAL_1(void fire(Rng& rng))
    SIGNAL_2(fire, rng)
};

class Css : public CsSignal::SlotBase
{
    NOINLINE(void handler(Rng& rng))
    {
        volatile std::size_t a = rng(); (void)a;
    }

    //std::deque<CsSignal::PendingSlot> *m_array;
    //std::mutex *m_mutex;
    //std::condition_variable *m_alarm;

    //bool compareThreads() const override;
    //void queueSlot(CsSignal::PendingSlot data, CsSignal::ConnectionKind type) override;

    public:

    using Signal = Css_Signal;

    template <typename Subject, typename Foo>
    static void connect_method(Subject& subject, Foo& foo)
    {
        CsSignal::connect(subject, &Subject::fire, foo, &Foo::handler);
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
    static double threaded(std::size_t, std::size_t);

    static constexpr const char* C_LIB_NAME = "copperspice cs_signal";
    static constexpr const char* C_LIB_SOURCE_URL = "https://github.com/copperspice/cs_signal";
    static constexpr const char* C_LIB_FILE = "benchmark_css";
    static constexpr const char* C_LIB_IS_HEADER_ONLY = "-";
    static constexpr const char* C_LIB_DATA_STRUCTURE = "**RCU List";
    static constexpr const char* C_LIB_IS_THREAD_SAFE = "DIY";

    static constexpr const std::size_t C_LIB_SIZEOF_SIGNAL = sizeof(Signal);
};
