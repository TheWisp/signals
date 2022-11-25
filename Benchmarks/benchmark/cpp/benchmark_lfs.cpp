#include "../hpp/benchmark_lfs.hpp"

NOINLINE(void Lfs::initialize())
{
    // NOOP
}
NOINLINE(void Lfs::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Lfs>::validation_assert(N);
}
NOINLINE(double Lfs::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lfs>::construction(N, limit);
}
NOINLINE(double Lfs::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lfs>::destruction(N, limit);
}
NOINLINE(double Lfs::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lfs>::connection(N, limit);
}
NOINLINE(double Lfs::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lfs>::disconnect(N, limit);
}
NOINLINE(double Lfs::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lfs>::reconnect(N, limit);
}
NOINLINE(double Lfs::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lfs>::emission(N, limit);
}
NOINLINE(double Lfs::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lfs>::combined(N, limit);
}
NOINLINE(double Lfs::threaded(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Lfs>::threaded(N, limit);
}
