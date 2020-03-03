#include "../hpp/benchmark_cps.hpp"

NOINLINE(void Cps::initialize())
{
    // NOOP
}
NOINLINE(void Cps::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Cps>::validation_assert(N);
}    
NOINLINE(double Cps::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps>::construction(N, limit);
}
NOINLINE(double Cps::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps>::destruction(N, limit);
}
NOINLINE(double Cps::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps>::connection(N, limit);
}
NOINLINE(double Cps::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps>::disconnect(N, limit);
}
NOINLINE(double Cps::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps>::reconnect(N, limit);
}
NOINLINE(double Cps::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps>::emission(N, limit);
}
NOINLINE(double Cps::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps>::combined(N, limit);
}
NOINLINE(double Cps::threaded(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps>::threaded(N, limit);
}
