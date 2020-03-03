#include "../hpp/benchmark_fts.hpp"

NOINLINE(void Fts::initialize())
{
    // NOOP
}
NOINLINE(void Fts::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Fts>::validation_assert(N);
}
NOINLINE(double Fts::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Fts>::construction(N, limit);
}
NOINLINE(double Fts::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Fts>::destruction(N, limit);
}
NOINLINE(double Fts::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Fts>::connection(N, limit);
}
NOINLINE(double Fts::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Fts>::disconnect(N, limit);
}
NOINLINE(double Fts::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Fts>::reconnect(N, limit);
}
NOINLINE(double Fts::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Fts>::emission(N, limit);
}
NOINLINE(double Fts::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Fts>::combined(N, limit);
}
NOINLINE(double Fts::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
