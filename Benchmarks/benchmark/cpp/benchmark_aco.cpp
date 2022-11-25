#include "../hpp/benchmark_aco.hpp"

NOINLINE(void Aco::initialize())
{
    // NOOP
}
NOINLINE(void Aco::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Aco>::validation_assert(N);
}
NOINLINE(double Aco::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Aco>::construction(N, limit);
}
NOINLINE(double Aco::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Aco>::destruction(N, limit);
}
NOINLINE(double Aco::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Aco>::connection(N, limit);
}
NOINLINE(double Aco::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Aco>::disconnect(N, limit);
}
NOINLINE(double Aco::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Aco>::reconnect(N, limit);
}
NOINLINE(double Aco::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Aco>::emission(N, limit);
}
NOINLINE(double Aco::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Aco>::combined(N, limit);
}
NOINLINE(double Aco::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
