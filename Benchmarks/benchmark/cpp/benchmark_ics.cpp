#include "../hpp/benchmark_ics.hpp"

#include <iscool/signals/signal.impl.tpp>

NOINLINE(void Ics::initialize())
{
    // NOOP
}
NOINLINE(void Ics::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Ics>::validation_assert(N);
}
NOINLINE(double Ics::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ics>::construction(N, limit);
}
NOINLINE(double Ics::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ics>::destruction(N, limit);
}
NOINLINE(double Ics::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ics>::connection(N, limit);
}
NOINLINE(double Ics::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ics>::disconnect(N, limit);
}
NOINLINE(double Ics::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ics>::reconnect(N, limit);
}
NOINLINE(double Ics::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ics>::emission(N, limit);
}
NOINLINE(double Ics::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ics>::combined(N, limit);
}
NOINLINE(double Ics::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
