#include "../hpp/benchmark_dob.hpp"

NOINLINE(void Dob::initialize())
{
    // NOOP
}
NOINLINE(void Dob::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Dob>::validation_assert(N);
}
NOINLINE(double Dob::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Dob>::construction(N, limit);
}
NOINLINE(double Dob::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Dob>::destruction(N, limit);
}
NOINLINE(double Dob::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Dob>::connection(N, limit);
}
NOINLINE(double Dob::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Dob>::disconnect(N, limit);
}
NOINLINE(double Dob::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Dob>::reconnect(N, limit);
}
NOINLINE(double Dob::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Dob>::emission(N, limit);
}
NOINLINE(double Dob::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Dob>::combined(N, limit);
}
NOINLINE(double Dob::threaded(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Dob>::threaded(N, limit);
}
