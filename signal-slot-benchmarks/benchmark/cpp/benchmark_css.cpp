#include "../hpp/benchmark_css.hpp"

NOINLINE(void Css::initialize())
{
    // NOOP
}
NOINLINE(void Css::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Css>::validation_assert(N);
}
NOINLINE(double Css::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Css>::construction(N, limit);
}
NOINLINE(double Css::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Css>::destruction(N, limit);
}
NOINLINE(double Css::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Css>::connection(N, limit);
}
NOINLINE(double Css::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Css>::disconnect(N, limit);
}
NOINLINE(double Css::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Css>::reconnect(N, limit);
}
NOINLINE(double Css::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Css>::emission(N, limit);
}
NOINLINE(double Css::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Css>::combined(N, limit);
}
NOINLINE(double Css::threaded(std::size_t N, std::size_t limit))
{
    //return Benchmark<Signal, Css>::threaded(N, limit);
    return 0.0;
}

//// This method will be called on the sending thread and should return true if the signal can be delivered in the same thread.
//bool Css::compareThreads() const
//{
//    return false;
//}
//
//// If compareThreads() returns false this method will be called.
//void Css::queueSlot(CsSignal::PendingSlot data, CsSignal::ConnectionKind type)
//{
//    // https://github.com/copperspice/cs_signal/blob/master/test/peach.cpp
//    std::lock_guard<std::mutex> lock(*m_mutex);
//    m_array->push_back(std::move(data));
//
//    // wake up the thread
//    m_alarm->notify_one();
//}
