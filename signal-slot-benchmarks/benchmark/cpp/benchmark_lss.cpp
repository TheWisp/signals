#include "../hpp/benchmark_lss.hpp"

NOINLINE(void Lss::initialize())
{
    // NOOP
}
NOINLINE(void Lss::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Lss>::validation_assert(N);
}
NOINLINE(double Lss::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lss>::construction(N, limit);
}
NOINLINE(double Lss::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lss>::destruction(N, limit);
}
NOINLINE(double Lss::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lss>::connection(N, limit);
}
NOINLINE(double Lss::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lss>::disconnect(N, limit);
}
NOINLINE(double Lss::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lss>::reconnect(N, limit);
}
NOINLINE(double Lss::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lss>::emission(N, limit);
}
NOINLINE(double Lss::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lss>::combined(N, limit);
}
NOINLINE(double Lss::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
