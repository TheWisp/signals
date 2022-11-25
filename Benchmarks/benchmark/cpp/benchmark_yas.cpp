#include "../hpp/benchmark_yas.hpp"

NOINLINE(void Yas::initialize())
{
    // NOOP
}
NOINLINE(void Yas::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Yas>::validation_assert(N);
}    
NOINLINE(double Yas::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Yas>::construction(N, limit);
}
NOINLINE(double Yas::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Yas>::destruction(N, limit);
}
NOINLINE(double Yas::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Yas>::connection(N, limit);
}
NOINLINE(double Yas::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Yas>::disconnect(N, limit);
}
NOINLINE(double Yas::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Yas>::reconnect(N, limit);
}
NOINLINE(double Yas::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Yas>::emission(N, limit);
}
NOINLINE(double Yas::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Yas>::combined(N, limit);
}
NOINLINE(double Yas::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
