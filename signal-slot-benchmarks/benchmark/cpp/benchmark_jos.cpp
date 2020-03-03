#include "../hpp/benchmark_jos.hpp"

NOINLINE(void Jos::initialize())
{
    // NOOP
}
NOINLINE(void Jos::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Jos>::validation_assert(N);
}    
NOINLINE(double Jos::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Jos>::construction(N, limit);
}
NOINLINE(double Jos::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Jos>::destruction(N, limit);
}
NOINLINE(double Jos::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Jos>::connection(N, limit);
}
NOINLINE(double Jos::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Jos>::disconnect(N, limit);
}
NOINLINE(double Jos::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Jos>::reconnect(N, limit);
}
NOINLINE(double Jos::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Jos>::emission(N, limit);
}
NOINLINE(double Jos::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Jos>::combined(N, limit);
}
NOINLINE(double Jos::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
