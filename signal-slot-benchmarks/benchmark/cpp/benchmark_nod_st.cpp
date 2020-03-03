#include "../hpp/benchmark_nod_st.hpp"

NOINLINE(void Nod_st::initialize())
{
    // NOOP
}
NOINLINE(void Nod_st::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Nod_st>::validation_assert(N);
}
NOINLINE(double Nod_st::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nod_st>::construction(N, limit);
}
NOINLINE(double Nod_st::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nod_st>::destruction(N, limit);
}
NOINLINE(double Nod_st::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nod_st>::connection(N, limit);
}
NOINLINE(double Nod_st::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nod_st>::disconnect(N, limit);
}
NOINLINE(double Nod_st::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nod_st>::reconnect(N, limit);
}
NOINLINE(double Nod_st::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nod_st>::emission(N, limit);
}
NOINLINE(double Nod_st::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nod_st>::combined(N, limit);
}
NOINLINE(double Nod_st::threaded(std::size_t N, std::size_t limit))
{
    return 0.0;
}
