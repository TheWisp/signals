#include "../hpp/benchmark_nss_ts.hpp"

NOINLINE(void Nss_ts::initialize())
{
    // NOOP
}
NOINLINE(void Nss_ts::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Nss_ts>::validation_assert(N);
}    
NOINLINE(double Nss_ts::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nss_ts>::construction(N, limit);
}
NOINLINE(double Nss_ts::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nss_ts>::destruction(N, limit);
}
NOINLINE(double Nss_ts::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nss_ts>::connection(N, limit);
}
NOINLINE(double Nss_ts::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nss_ts>::disconnect(N, limit);
}
NOINLINE(double Nss_ts::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nss_ts>::reconnect(N, limit);
}
NOINLINE(double Nss_ts::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nss_ts>::emission(N, limit);
}
NOINLINE(double Nss_ts::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nss_ts>::combined(N, limit);
}
NOINLINE(double Nss_ts::threaded(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Nss_ts>::threaded(N, limit);
}
