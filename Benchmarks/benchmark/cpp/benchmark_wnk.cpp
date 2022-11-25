#include "../hpp/benchmark_wnk.hpp"

NOINLINE(void Wnk::initialize())
{
    // NOOP
}
NOINLINE(void Wnk::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Wnk>::validation_assert(N);
}    
NOINLINE(double Wnk::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Wnk>::construction(N, limit);
}
NOINLINE(double Wnk::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Wnk>::destruction(N, limit);
}
NOINLINE(double Wnk::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Wnk>::connection(N, limit);
}
NOINLINE(double Wnk::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Wnk>::disconnect(N, limit);
}
NOINLINE(double Wnk::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Wnk>::reconnect(N, limit);
}
NOINLINE(double Wnk::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Wnk>::emission(N, limit);
}
NOINLINE(double Wnk::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Wnk>::combined(N, limit);
}
NOINLINE(double Wnk::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
