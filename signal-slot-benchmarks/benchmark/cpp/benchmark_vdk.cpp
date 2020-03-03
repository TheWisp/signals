#include "../hpp/benchmark_vdk.hpp"

NOINLINE(void Vdk::initialize())
{
    // NOOP
}
NOINLINE(void Vdk::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Vdk>::validation_assert(N);
}    
NOINLINE(double Vdk::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Vdk>::construction(N, limit);
}
NOINLINE(double Vdk::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Vdk>::destruction(N, limit);
}
NOINLINE(double Vdk::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Vdk>::connection(N, limit);
}
NOINLINE(double Vdk::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Vdk>::disconnect(N, limit);
}
NOINLINE(double Vdk::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Vdk>::reconnect(N, limit);
}
NOINLINE(double Vdk::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Vdk>::emission(N, limit);
}
NOINLINE(double Vdk::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Vdk>::combined(N, limit);
}
NOINLINE(double Vdk::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
