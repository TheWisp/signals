#include <filesystem>
#include <fstream>
#include <iomanip>

#include "benchmark/hpp/benchmark_aco.hpp"
#include "benchmark/hpp/benchmark_asg.hpp"
#include "benchmark/hpp/benchmark_bs2.hpp"
#include "benchmark/hpp/benchmark_cls.hpp"
#include "benchmark/hpp/benchmark_cps.hpp"
#include "benchmark/hpp/benchmark_cps_st.hpp"
#include "benchmark/hpp/benchmark_css.hpp"
#include "benchmark/hpp/benchmark_dob.hpp"
#include "benchmark/hpp/benchmark_evl.hpp"
#include "benchmark/hpp/benchmark_ics.hpp"
#include "benchmark/hpp/benchmark_jls.hpp"
#include "benchmark/hpp/benchmark_jos.hpp"
#include "benchmark/hpp/benchmark_ksc.hpp"
#include "benchmark/hpp/benchmark_lfs.hpp"
#include "benchmark/hpp/benchmark_lss.hpp"
#include "benchmark/hpp/benchmark_mws.hpp"
#include "benchmark/hpp/benchmark_nes.hpp"
#include "benchmark/hpp/benchmark_nls.hpp"
#include "benchmark/hpp/benchmark_nls_st.hpp"
#include "benchmark/hpp/benchmark_nod.hpp"
#include "benchmark/hpp/benchmark_nod_st.hpp"
#include "benchmark/hpp/benchmark_nss_st.hpp"
#include "benchmark/hpp/benchmark_nss_sts.hpp"
#include "benchmark/hpp/benchmark_nss_ts.hpp"
#include "benchmark/hpp/benchmark_nss_tss.hpp"
#include "benchmark/hpp/benchmark_psg.hpp"
#include "benchmark/hpp/benchmark_pss.hpp"
#include "benchmark/hpp/benchmark_pss_st.hpp"
#include "benchmark/hpp/benchmark_sss.hpp"
#include "benchmark/hpp/benchmark_wnk.hpp"
#include "benchmark/hpp/benchmark_yas.hpp"
#include "benchmark/hpp/benchmark_vdk.hpp"
#include "benchmark/hpp/benchmark_fts.hpp"

// These are required in Main to initialize the jlsignal allocator
#include "benchmark/lib/jeffomatic/jl_signal/src/Signal.h"
#include "benchmark/lib/jeffomatic/jl_signal/src/StaticSignalConnectionAllocators.h"

#include "benchmark/lib/NoAvailableAlias/nano-signal-slot/nano_mutex.hpp"

#include "benchmark_utility.hpp"

//------------------------------------------------------------------------------

using Mutex = Nano::Spin_Mutex;
using Lock_Guard = std::lock_guard<Mutex>;

//------------------------------------------------------------------------------

// The amount of time to accumulate per benchmark method sample
static std::size_t s_timer_limit = Timer_u(Limit_u(10)).count();
// Number of times each benchmark method is run per sample
static std::size_t s_best_of_limit = 2;
// Starting size of "N test size"
static std::size_t s_start_test_size = 2;
// The maximum test size of "N"
static std::size_t s_ending_test_size = 64;
// Store the previous full results of a particular converging run
static BenchmarkClassResults s_current_results;
// Main synchronization for copying s_current_results to the IO thread
static Mutex s_io_mutex;
// Used to early exit out of a currently running benchmark
static std::atomic_bool s_esc_pressed { false };

//------------------------------------------------------------------------------

template <auto fun_ptr>
double best_of(std::size_t N)
{
    std::vector<double> run_results;
    for (std::size_t i = 0; i < s_best_of_limit; ++i)
    {
        run_results.push_back((*fun_ptr)(N, s_timer_limit));
    }
    return *std::max_element(run_results.begin(), run_results.end());
}

bool post_benchmark_method_update(BenchmarkClassResults const& results)
{
    {
        Lock_Guard lock(s_io_mutex);
        s_current_results = results;
    }
    return s_esc_pressed.load();
}

template <typename Benchmark>
bool run_benchmark_class(BenchmarkClassResults& results, std::size_t N)
{
#define POST_BENCHMARK_METHOD_UPDATE(...) if (post_benchmark_method_update(results)) return true

    try
    {
        auto& lib_results = results[Benchmark::C_LIB_NAME];

        Benchmark::initialize();

        lib_results[C_CONSTRUCTION].emplace_back(N, best_of<Benchmark::construction>(N));
        POST_BENCHMARK_METHOD_UPDATE();
        lib_results[C_DESTRUCTION].emplace_back(N, best_of<Benchmark::destruction>(N));
        POST_BENCHMARK_METHOD_UPDATE();
        lib_results[C_CONNECTION].emplace_back(N, best_of<Benchmark::connection>(N));
        POST_BENCHMARK_METHOD_UPDATE();
        lib_results[C_DISCONNECT].emplace_back(N, best_of<Benchmark::disconnect>(N));
        POST_BENCHMARK_METHOD_UPDATE();
        lib_results[C_RECONNECT].emplace_back(N, best_of<Benchmark::reconnect>(N));
        POST_BENCHMARK_METHOD_UPDATE();
        lib_results[C_EMISSION].emplace_back(N, best_of<Benchmark::emission>(N));
        POST_BENCHMARK_METHOD_UPDATE();
        lib_results[C_COMBINED].emplace_back(N, best_of<Benchmark::combined>(N));
        POST_BENCHMARK_METHOD_UPDATE();
        lib_results[C_THREADED].emplace_back(N, best_of<Benchmark::threaded>(N));
        POST_BENCHMARK_METHOD_UPDATE();
    }
    catch (std::exception const& error)
    {
        std::cerr << "Exception: " << error.what() << std::endl;
    }
    return false;

#undef POST_BENCHMARK_METHOD_UPDATE
}

//------------------------------------------------------------------------------

void run_all_benchmarks()
{
#define RUN_BENCHMARK_CLASS(FOO) if (run_benchmark_class<FOO>(results, N)) return

    BenchmarkClassResults results;
    {
        Lock_Guard lock(s_io_mutex);
        results = s_current_results;
    }

    // Double the test size N for every iteration
    for (auto N = s_start_test_size; N <= s_ending_test_size; N *= 2)
    {
        RUN_BENCHMARK_CLASS(Aco);
        RUN_BENCHMARK_CLASS(Asg);
        RUN_BENCHMARK_CLASS(Bs2);
        RUN_BENCHMARK_CLASS(Cls); // Must make sure Subject lives longer than Foo
        RUN_BENCHMARK_CLASS(Cps);
        RUN_BENCHMARK_CLASS(Cps_st);
        RUN_BENCHMARK_CLASS(Css);
        RUN_BENCHMARK_CLASS(Dob); // Must make sure Subject lives longer than Foo
        RUN_BENCHMARK_CLASS(Evl);
        RUN_BENCHMARK_CLASS(Fts);
        RUN_BENCHMARK_CLASS(Ics);
        RUN_BENCHMARK_CLASS(Jls);
        RUN_BENCHMARK_CLASS(Jos); // Must make sure Subject lives longer than Foo
        RUN_BENCHMARK_CLASS(Ksc);
        RUN_BENCHMARK_CLASS(Lfs);
        RUN_BENCHMARK_CLASS(Lss); // Must make sure Subject lives longer than Foo
        RUN_BENCHMARK_CLASS(Mws);
        RUN_BENCHMARK_CLASS(Nes);
        RUN_BENCHMARK_CLASS(Nls);
        RUN_BENCHMARK_CLASS(Nls_st);
        RUN_BENCHMARK_CLASS(Nod);
        RUN_BENCHMARK_CLASS(Nod_st);
        RUN_BENCHMARK_CLASS(Nss_st);
        RUN_BENCHMARK_CLASS(Nss_sts);
        RUN_BENCHMARK_CLASS(Nss_ts);
        RUN_BENCHMARK_CLASS(Nss_tss);
        RUN_BENCHMARK_CLASS(Psg);
        RUN_BENCHMARK_CLASS(Pss);
        RUN_BENCHMARK_CLASS(Pss_st);
        RUN_BENCHMARK_CLASS(Sss);
        RUN_BENCHMARK_CLASS(Wnk); // Must make sure Subject lives longer than Foo
        RUN_BENCHMARK_CLASS(Yas);
        RUN_BENCHMARK_CLASS(Vdk);
    }

#undef RUN_BENCHMARK_CLASS
}

//------------------------------------------------------------------------------

void run_all_validation_tests()
{
    auto N = s_ending_test_size;

    Aco::validate_assert(N);
    Asg::validate_assert(N);
    Bs2::validate_assert(N); // Boost::signals2 is not passing validation fsr
    Cls::validate_assert(N);
    Cps::validate_assert(N);
    Cps_st::validate_assert(N);
    Css::validate_assert(N);
    Dob::validate_assert(N);
    Evl::validate_assert(N);
    Fts::validate_assert(N);
    Ics::validate_assert(N);
    Jls::validate_assert(N);
    Jos::validate_assert(N);
    Ksc::validate_assert(N);
    Lfs::validate_assert(N);
    Lss::validate_assert(N);
    Mws::validate_assert(N);
    Nes::validate_assert(N);
    Nls::validate_assert(N);
    Nls_st::validate_assert(N);
    Nod::validate_assert(N);
    Nod_st::validate_assert(N);
    Nss_st::validate_assert(N);
    Nss_sts::validate_assert(N);
    Nss_ts::validate_assert(N);
    Nss_tss::validate_assert(N);
    Psg::validate_assert(N);
    Pss::validate_assert(N);
    Pss_st::validate_assert(N);
    Sss::validate_assert(N);
    Wnk::validate_assert(N);
    Yas::validate_assert(N);
    Vdk::validate_assert(N);
}

//------------------------------------------------------------------------------

std::string get_object_file_size(const char* file_stem)
{
    namespace fs = std::filesystem;

#if defined(__GNUC__) || defined(__GNUG__)
    auto file_name = std::string(file_stem) + ".o";
#else
    auto file_name = std::string(file_stem) + ".obj";
#endif

    const fs::path file { file_name };

    if (fs::exists(file))
    {
        return std::to_string(fs::file_size(file) / 1000) + " kb";
    }
    return "&mdash;";
}

template <typename Benchmark, typename T>
void output_metrics_report_row(T& ost)
{
    try
    {
        ost << "| [" << Benchmark::C_LIB_NAME << "]("
            << Benchmark::C_LIB_SOURCE_URL << ") | "
            << get_object_file_size(Benchmark::C_LIB_FILE)
            << " | " << Benchmark::C_LIB_SIZEOF_SIGNAL << " b"
            << " | " << Benchmark::C_LIB_IS_HEADER_ONLY
            << " | " << Benchmark::C_LIB_DATA_STRUCTURE
            << " | " << Benchmark::C_LIB_IS_THREAD_SAFE << " |\n";
    }
    catch (std::exception const& error)
    {
        std::cerr << "Exception: " << error.what() << std::endl;
    }
}

template <typename T>
void output_metrics_report(T& ost)
{
    ost << "\n| Library | Build Size | Signal Size | Header Only | Data Structure | Thread Safe |\n"
        << "| ------- |:----------:|:-----------:|:-----------:| -------------- |:-----------:|\n";

    output_metrics_report_row<Aco>(ost);
    output_metrics_report_row<Asg>(ost);
    output_metrics_report_row<Bs2>(ost);
    output_metrics_report_row<Cls>(ost);
    output_metrics_report_row<Cps>(ost);
    output_metrics_report_row<Cps_st>(ost);
    output_metrics_report_row<Css>(ost);
    output_metrics_report_row<Dob>(ost);
    output_metrics_report_row<Evl>(ost);
    output_metrics_report_row<Fts>(ost);
    output_metrics_report_row<Ics>(ost);
    output_metrics_report_row<Jls>(ost);
    output_metrics_report_row<Jos>(ost);
    output_metrics_report_row<Ksc>(ost);
    output_metrics_report_row<Lfs>(ost);
    output_metrics_report_row<Lss>(ost);
    output_metrics_report_row<Mws>(ost);
    output_metrics_report_row<Nes>(ost);
    output_metrics_report_row<Nls>(ost);
    output_metrics_report_row<Nls_st>(ost);
    output_metrics_report_row<Nod>(ost);
    output_metrics_report_row<Nod_st>(ost);
    output_metrics_report_row<Nss_st>(ost);
    output_metrics_report_row<Nss_sts>(ost);
    output_metrics_report_row<Nss_ts>(ost);
    output_metrics_report_row<Nss_tss>(ost);
    output_metrics_report_row<Psg>(ost);
    output_metrics_report_row<Pss>(ost);
    output_metrics_report_row<Pss_st>(ost);
    output_metrics_report_row<Sss>(ost);
    output_metrics_report_row<Wnk>(ost);
    output_metrics_report_row<Yas>(ost);
    output_metrics_report_row<Vdk>(ost);
}

//------------------------------------------------------------------------------

template <typename T>
void output_plotly_reports(BenchmarkClassResults const& records, T& ost)
{
    std::string lib_columns;
    for (auto const& [lib_name, results] : records)
    {
        lib_columns += std::string("\"") + std::string(lib_name) + ", y;\",";
    }
    ost << "\n\"Test Size N, x;\"," << lib_columns;

    for (auto c = s_start_test_size; c <= s_ending_test_size; c *= 2)
    {
        ost << "\n" << std::to_string(c) << ",";
        for (auto const& [lib_name, results] : records)
        {
            auto score = 0.0;
            auto sum = 0.0;
            auto ops = 0;

            for (auto const& [op_name, vals] : results)
            {
                if (op_name[0] == '[')
                {
                    continue;
                }

                for (auto const& [N, result] : vals)
                {
                    if (c == N)
                    {
                        sum += result;
                    }
                }
                ++ops;
            }
            score = sum / (double)ops;
            ost << std::to_string(score) << ",";
        }
    }
    ost << "\n";
}

//------------------------------------------------------------------------------

template <typename T>
void output_perf_report_header(ReportMethodResults const& first_result_row, T& ost)
{
    std::string header_first_row("| Library | ");

    for (auto const& [op_name, result] : first_result_row)
    {
        header_first_row.append(op_name);
        header_first_row.append(" | ");
    }
    header_first_row += "score |";

    ost << "\n" << header_first_row << "\n";

    for (auto const& ch : header_first_row)
    {
        ost << (ch == '|' ? '|' : '-');
    }
    ost << "\n";
}

//------------------------------------------------------------------------------

template <typename T>
void output_reports(BenchmarkClassResults const& records, T& ost)
{
    ReportClassResults result_average;
    ReportOrderedResults result_order;

    // Process and sort results by total score (sum of column averages)

    for (auto const& [lib_name, results] : records)
    {
        auto score = 0.0;

        for (auto const& [op_name, vals] : results)
        {
            auto sum = 0.0;
            for (auto const& [N, result] : vals)
            {
                sum += result;
            }
            auto average = sum / (double)vals.size();

            result_average[lib_name][op_name] = average;

            if (op_name[0] != '[')
            {
                score += average;
            }
        }
        result_order[score] = ReportOrderedResult { lib_name, &result_average[lib_name] };
    }

    bool show_header = true;

    for (auto const& row : Range(result_order.rbegin(), result_order.rend()))
    {
        auto const& score = row.first;
        auto const& lib_name = row.second.first;

        if (show_header)
        {
            output_perf_report_header((*row.second.second), ost);
            show_header = false;
        }

        ost << "| " << lib_name;

        for (auto const& column : (*row.second.second))
        {
            auto const& val = column.second;

            ost << " | " << std::setprecision(0) << std::fixed << val;
        }
        ost << " | " << std::setprecision(0) << std::fixed << score << " |\n";
    }
}

//------------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    // Jl_signal uses a static allocator for maximum performance
    jl::StaticSignalConnectionAllocator<C_JLSIGNAL_MAX> signal_con_allocator;
    jl::StaticObserverConnectionAllocator<C_JLSIGNAL_MAX> observer_con_allocator;
    jl::SignalBase::SetCommonConnectionAllocator(&signal_con_allocator);
    jl::SignalObserver::SetCommonConnectionAllocator(&observer_con_allocator);

    // Make sure to set the process to high priority
    std::cout << "Change the CPU priority now: [paused]" << std::endl;
    std::cin.get();

    // Time the entire duration of the benchmark process
    auto start = std::chrono::system_clock::now();

    // Validate that all implementations are functioning correctly
    run_all_validation_tests();

    // There has got to be a better way...
    auto a1 = std::async(std::launch::async, []
    {
        auto a2 = std::async(std::launch::async, []
        {
            return std::cin.get();
        });

        std::future_status s1;

        do
        {
            s1 = a2.wait_for(std::chrono::seconds(1));

            if (s1 != std::future_status::ready)
            {
#if defined(_WIN32)
                system("cls");
#else
                std::cout << "\033c" << std::endl;
#endif
                BenchmarkClassResults results;
                {
                    Lock_Guard lock(s_io_mutex);
                    results = s_current_results;
                }
                output_reports(results, std::cout);
            }
        }
        while (s1 != std::future_status::ready);
        s_esc_pressed.store(true);
        a2.get();
    });

    // Increase the accuracy of each subsequent run
    for (; !s_esc_pressed.load(); s_timer_limit *= 2, s_best_of_limit *= 2)
    {
        run_all_benchmarks();
    }
    a1.get();

    if (std::ofstream ofs{ "report.txt", std::ios::app })
    {
        auto start_c = std::chrono::system_clock::to_time_t(start);

        auto tee = std::tie(std::cout, ofs);
        tee << "\n" << std::put_time(std::localtime(&start_c), "%c") << "\n";

        output_reports(s_current_results, tee);
        output_metrics_report(tee);
        output_plotly_reports(s_current_results, tee);

        auto stop = std::chrono::system_clock::now();
        auto stop_c = std::chrono::system_clock::to_time_t(stop);
        tee << "\n" << std::put_time(std::localtime(&stop_c), "%c") << "\n";

        ofs << std::endl;
    }
    std::cin.get();
}
