#include "../hpp/benchmark_sss.hpp"

NOINLINE(void Sss::initialize())
{
    // NOOP
}
NOINLINE(void Sss::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Sss>::validation_assert(N);
}    
NOINLINE(double Sss::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Sss>::construction(N, limit);
}
NOINLINE(double Sss::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Sss>::destruction(N, limit);
}
NOINLINE(double Sss::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Sss>::connection(N, limit);
}
NOINLINE(double Sss::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Sss>::disconnect(N, limit);
}
NOINLINE(double Sss::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Sss>::reconnect(N, limit);
}
NOINLINE(double Sss::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Sss>::emission(N, limit);
}
NOINLINE(double Sss::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Sss>::combined(N, limit);
}
NOINLINE(double Sss::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
