#include "../hpp/benchmark_asg.hpp"

NOINLINE(void Asg::initialize())
{
    // NOOP
}
NOINLINE(void Asg::validate_assert(std::size_t N))
{
    return Benchmark<Signal, Asg>::validation_assert(N);
}    
NOINLINE(double Asg::construction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Asg>::construction(N, limit);
}
NOINLINE(double Asg::destruction(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Asg>::destruction(N, limit);
}
NOINLINE(double Asg::connection(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Asg>::connection(N, limit);
}
NOINLINE(double Asg::disconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Asg>::disconnect(N, limit);
}
NOINLINE(double Asg::reconnect(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Asg>::reconnect(N, limit);
}
NOINLINE(double Asg::emission(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Asg>::emission(N, limit);
}
NOINLINE(double Asg::combined(std::size_t N, std::size_t limit))
{
    return Benchmark<Signal, Asg>::combined(N, limit);
}
NOINLINE(double Asg::threaded(std::size_t N, std::size_t limit))
{
    // NOT IMPLEMENTED FOR THIS LIB
    return 0.0;
}
