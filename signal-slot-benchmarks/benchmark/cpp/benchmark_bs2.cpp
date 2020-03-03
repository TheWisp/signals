#include "../hpp/benchmark_bs2.hpp"

NOINLINE(void Bs2::initialize())
{
    // NOOP
}
NOINLINE(void Bs2::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Bs2>::validation_assert(N);
}    
NOINLINE(double Bs2::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Bs2>::construction(N, limit);
}
NOINLINE(double Bs2::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Bs2>::destruction(N, limit);
}
NOINLINE(double Bs2::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Bs2>::connection(N, limit);
}
NOINLINE(double Bs2::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Bs2>::disconnect(N, limit);
}
NOINLINE(double Bs2::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Bs2>::reconnect(N, limit);
}
NOINLINE(double Bs2::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Bs2>::emission(N, limit);
}
NOINLINE(double Bs2::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Bs2>::combined(N, limit);
}
NOINLINE(double Bs2::threaded(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Bs2>::threaded(N, limit);
}
