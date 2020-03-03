#include "../hpp/benchmark_pss_st.hpp"

NOINLINE(void Pss_st::initialize())
{
    // NOOP
}
NOINLINE(void Pss_st::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Pss_st>::validation_assert(N);
}
NOINLINE(double Pss_st::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss_st>::construction(N, limit);
}
NOINLINE(double Pss_st::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss_st>::destruction(N, limit);
}
NOINLINE(double Pss_st::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss_st>::connection(N, limit);
}
NOINLINE(double Pss_st::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss_st>::disconnect(N, limit);
}
NOINLINE(double Pss_st::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss_st>::reconnect(N, limit);
}
NOINLINE(double Pss_st::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss_st>::emission(N, limit);
}
NOINLINE(double Pss_st::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Pss_st>::combined(N, limit);
}
NOINLINE(double Pss_st::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
