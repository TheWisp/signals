#include "../hpp/benchmark_evl.hpp"

NOINLINE(void Evl::initialize())
{
    // NOOP
}
NOINLINE(void Evl::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Evl>::validation_assert(N);
}    
NOINLINE(double Evl::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Evl>::construction(N, limit);
}
NOINLINE(double Evl::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Evl>::destruction(N, limit);
}
NOINLINE(double Evl::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Evl>::connection(N, limit);
}
NOINLINE(double Evl::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Evl>::disconnect(N, limit);
}
NOINLINE(double Evl::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Evl>::reconnect(N, limit);
}
NOINLINE(double Evl::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Evl>::emission(N, limit);
}
NOINLINE(double Evl::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Evl>::combined(N, limit);
}
NOINLINE(double Evl::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
