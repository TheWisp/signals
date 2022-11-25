#pragma once

#include <algorithm>
#include <iostream>
#include <cassert>
#include <numeric>
#include <chrono>
#include <future>
#include <memory>
#include <random>
#include <vector>
#include <tuple>
#include <map>

#ifdef _WIN32
#define NOINLINE(s) __declspec(noinline) s
#elif __unix__
#define NOINLINE(s) __attribute__ ((noinline)) s
#else
#define NOINLINE(s) s
#endif

typedef std::minstd_rand Rng;

// Time units used internally in the benchmarks
typedef std::chrono::nanoseconds Timer_u;
typedef std::chrono::milliseconds Limit_u;
typedef std::chrono::duration<double, std::milli> Delta_u;

// Used for gathering raw lib benchmark scores
typedef std::pair<std::size_t, double> BenchmarkRawResult;
typedef std::map<const char*, std::vector<BenchmarkRawResult>> BenchmarkMethodResults;
typedef std::map<const char*, BenchmarkMethodResults> BenchmarkClassResults;

// Used for post-benchmark processing and report output
typedef std::map<const char*, double> ReportMethodResults;
typedef std::map<const char*, ReportMethodResults> ReportClassResults;
typedef std::pair<const char*, ReportMethodResults*> ReportOrderedResult;
typedef std::map<double, ReportOrderedResult> ReportOrderedResults;

// Used for the initialization of jlsignal
constexpr const std::size_t C_JLSIGNAL_MAX = 256;

// Constants used to map to a particular benchmark method
constexpr const char* C_CONSTRUCTION = "[constr]";
constexpr const char* C_DESTRUCTION = "[destr]";
constexpr const char* C_CONNECTION = "conn";
constexpr const char* C_DISCONNECT = "disconn";
constexpr const char* C_RECONNECT = "reconn";
constexpr const char* C_EMISSION = "emit";
constexpr const char* C_COMBINED = "all";
constexpr const char* C_THREADED = "threaded";

//------------------------------------------------------------------------------

typedef std::shared_ptr<void> SlotScope;

template <typename Deleter>
SlotScope make_slot_scope(Deleter&& deleter)
{
    return SlotScope(nullptr, deleter);
}

//------------------------------------------------------------------------------

template <unsigned int N>
struct tee_stream
{
    template <typename ...Args, typename T>
    static std::tuple<Args...>& print(std::tuple<Args...>& t, T&& x)
    {
        std::get<sizeof...(Args) - N>(t) << x;
        tee_stream<N - 1>::print(t, std::forward<T>(x));
        return t;
    }
};
template <>
struct tee_stream<0>
{
    template <typename ...Args, typename T>
    static std::tuple<Args...>& print(std::tuple<Args...>& t, T&&)
    {
        return t;
    }
};
template <typename ...Args, typename T>
std::tuple<Args...>& operator<< (std::tuple<Args...>& t, T&& x)
{
    return tee_stream<sizeof...(Args)>::print(t, std::forward<T>(x));
}
template <typename ...Args, typename T>
std::tuple<Args...>& operator<< (std::tuple<Args...>&& t, T&& x)
{
    return tee_stream<sizeof...(Args)>::print(t, std::forward<T>(x));
}

//------------------------------------------------------------------------------

template <typename Iterator> struct RangeHelper
{
    Iterator lhs;
    Iterator rhs;

    Iterator cbegin() const { return lhs; }
    Iterator begin() const { return lhs; }
    Iterator cend() const { return rhs; }
    Iterator end() const { return rhs; }
};
template <typename Iterator>
static RangeHelper<Iterator> Range(Iterator lhs,
                                   Iterator rhs)
{
    return { lhs, rhs };
}

//------------------------------------------------------------------------------

class ChronoTimer
{
    std::chrono::time_point<std::chrono::high_resolution_clock> m_start;

    public:

    ChronoTimer() : m_start(std::chrono::high_resolution_clock::now())
    {
    
    }

    void reset()
    {
        new (this) ChronoTimer;
    }
    template <typename T>
    std::size_t count() const
    {
        return std::chrono::duration_cast<T>
          (std::chrono::high_resolution_clock::now() - m_start).count();
    }
};

//------------------------------------------------------------------------------

struct IncrementFill
{
    std::size_t i = 0;
    std::size_t operator()() { return i++; }
};
