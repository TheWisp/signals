#include "../hpp/benchmark_psg.hpp"

NOINLINE(void Psg::initialize())
{
    // NOOP
}
NOINLINE(void Psg::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Psg>::validation_assert(N);
}    
NOINLINE(double Psg::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Psg>::construction(N, limit);
}
NOINLINE(double Psg::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Psg>::destruction(N, limit);
}
NOINLINE(double Psg::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Psg>::connection(N, limit);
}
NOINLINE(double Psg::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Psg>::disconnect(N, limit);
}
NOINLINE(double Psg::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Psg>::reconnect(N, limit);
}
NOINLINE(double Psg::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Psg>::emission(N, limit);
}
NOINLINE(double Psg::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Psg>::combined(N, limit);
}
NOINLINE(double Psg::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
