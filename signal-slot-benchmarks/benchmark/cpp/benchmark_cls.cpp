#include "../hpp/benchmark_cls.hpp"

NOINLINE(void Cls::initialize())
{
    // NOOP
}
NOINLINE(void Cls::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Cls>::validation_assert(N);
}    
NOINLINE(double Cls::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cls>::construction(N, limit);
}
NOINLINE(double Cls::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cls>::destruction(N, limit);
}
NOINLINE(double Cls::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cls>::connection(N, limit);
}
NOINLINE(double Cls::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cls>::disconnect(N, limit);
}
NOINLINE(double Cls::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cls>::reconnect(N, limit);
}
NOINLINE(double Cls::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cls>::emission(N, limit);
}
NOINLINE(double Cls::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cls>::combined(N, limit);
}
NOINLINE(double Cls::threaded(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cls>::threaded(N, limit);
}
