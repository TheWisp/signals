#include "../hpp/benchmark_cps_st.hpp"

NOINLINE(void Cps_st::initialize())
{
    // NOOP
}
NOINLINE(void Cps_st::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Cps_st>::validation_assert(N);
}
NOINLINE(double Cps_st::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps_st>::construction(N, limit);
}
NOINLINE(double Cps_st::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps_st>::destruction(N, limit);
}
NOINLINE(double Cps_st::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps_st>::connection(N, limit);
}
NOINLINE(double Cps_st::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps_st>::disconnect(N, limit);
}
NOINLINE(double Cps_st::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps_st>::reconnect(N, limit);
}
NOINLINE(double Cps_st::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps_st>::emission(N, limit);
}
NOINLINE(double Cps_st::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Cps_st>::combined(N, limit);
}
NOINLINE(double Cps_st::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
