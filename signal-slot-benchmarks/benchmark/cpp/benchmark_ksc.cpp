#include "../hpp/benchmark_ksc.hpp"

NOINLINE(void Ksc::initialize())
{
    // NOOP
}
NOINLINE(void Ksc::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Ksc>::validation_assert(N);
}    
NOINLINE(double Ksc::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ksc>::construction(N, limit);
}
NOINLINE(double Ksc::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ksc>::destruction(N, limit);
}
NOINLINE(double Ksc::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ksc>::connection(N, limit);
}
NOINLINE(double Ksc::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ksc>::disconnect(N, limit);
}
NOINLINE(double Ksc::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ksc>::reconnect(N, limit);
}
NOINLINE(double Ksc::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ksc>::emission(N, limit);
}
NOINLINE(double Ksc::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ksc>::combined(N, limit);
}
NOINLINE(double Ksc::threaded(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Ksc>::threaded(N, limit);
}
