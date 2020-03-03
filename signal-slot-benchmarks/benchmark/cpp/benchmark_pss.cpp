#include "../hpp/benchmark_pss.hpp"

NOINLINE(void Pss::initialize())
{
    // NOOP
}
NOINLINE(void Pss::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Pss>::validation_assert(N);
}
NOINLINE(double Pss::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss>::construction(N, limit);
}
NOINLINE(double Pss::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss>::destruction(N, limit);
}
NOINLINE(double Pss::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss>::connection(N, limit);
}
NOINLINE(double Pss::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss>::disconnect(N, limit);
}
NOINLINE(double Pss::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss>::reconnect(N, limit);
}
NOINLINE(double Pss::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss>::emission(N, limit);
}
NOINLINE(double Pss::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss>::combined(N, limit);
}
NOINLINE(double Pss::threaded(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss>::threaded(N, limit);
}
