#include "../hpp/benchmark_mws.hpp"

NOINLINE(void Mws::initialize())
{
    // NOOP
}
NOINLINE(void Mws::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Mws>::validation_assert(N);
}    
NOINLINE(double Mws::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Mws>::construction(N, limit);
}
NOINLINE(double Mws::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Mws>::destruction(N, limit);
}
NOINLINE(double Mws::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Mws>::connection(N, limit);
}
NOINLINE(double Mws::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Mws>::disconnect(N, limit);
}
NOINLINE(double Mws::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Mws>::reconnect(N, limit);
}
NOINLINE(double Mws::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Mws>::emission(N, limit);
}
NOINLINE(double Mws::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Mws>::combined(N, limit);
}
NOINLINE(double Mws::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
