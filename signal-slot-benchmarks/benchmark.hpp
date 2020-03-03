#pragma once

#include "benchmark_utility.hpp"

template <typename Subject, typename Foo>
class Benchmark
{
    static ChronoTimer s_timer;
    static Rng s_rng;

    static double calculate_score(std::size_t N, std::size_t limit, std::size_t count)
    {
        // Return the test size N divided by (the sample size / number of samples)
        return N / std::chrono::duration_cast<Delta_u>(Timer_u(limit / count)).count();
    }

    public:

    static void validation_assert(std::size_t N)
    {
        Rng rng;
        std::size_t count = 0;

        for (auto i = N; i != 0; --i, count += N)
        {
            Subject subject;
            {
                std::vector<Foo> foo_array(N);

                for (auto& foo : foo_array)
                {
                    Foo::connect_method(subject, foo);
                }
                Foo::emit_method(subject, rng);
            }
            // Make sure disconnect is working correctly
            Foo::emit_method(subject, rng);
        }
        // If the PRNG state is different
        // There is an error in the signal implementation
        Rng test;
        test.discard(count);
        if (rng != test) std::cerr << "Validation Failed: " << Foo::C_LIB_NAME << std::endl;
    }

    //--------------------------------------------------------------------------

    static double construction(std::size_t N, std::size_t limit)
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;

        for (; elapsed < limit; ++count)
        {
            s_timer.reset();

            std::vector<Subject> subjects(N * N);

            elapsed += s_timer.count<Timer_u>();
        }
        return calculate_score(N, elapsed, count);
    }

    //--------------------------------------------------------------------------

    static double destruction(std::size_t N, std::size_t limit)
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;

        for (; elapsed < limit; ++count)
        {
            {
                std::vector<Subject> subjects(N * N);

                s_timer.reset();
            }
            elapsed += s_timer.count<Timer_u>();
        }
        return calculate_score(N, elapsed, count);
    }

    //--------------------------------------------------------------------------

    static double connection(std::size_t N, std::size_t limit)
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;

        for (; elapsed < limit; ++count)
        {
            Subject subject;
            std::vector<Foo> foo(N);

            s_timer.reset();

            for (auto& foo_instance : foo)
            {
                Foo::connect_method(subject, foo_instance);
            }
            elapsed += s_timer.count<Timer_u>();
        }
        return calculate_score(N, elapsed, count);
    }

    //--------------------------------------------------------------------------

    static double disconnect(std::size_t N, std::size_t limit)
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;

        for (; elapsed < limit; ++count)
        {
            Subject subject;
            {
                std::vector<Foo> foo(N);

                for (auto& foo_instance : foo)
                {
                    Foo::connect_method(subject, foo_instance);
                }
                s_timer.reset();

                // Disconnect N Foo from Subject
            }
            elapsed += s_timer.count<Timer_u>();
        }
        return calculate_score(N, elapsed, count);
    }


    //--------------------------------------------------------------------------

    static double reconnect(std::size_t N, std::size_t limit)
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;

        Subject subject;

        for (; elapsed < limit; ++count)
        {
            std::vector<Foo> foo(N);

            s_timer.reset();

            for (auto& foo_instance : foo)
            {
                Foo::connect_method(subject, foo_instance);
            }
            elapsed += s_timer.count<Timer_u>();
        }
        return calculate_score(N, elapsed, count);
    }

    //--------------------------------------------------------------------------

    static double emission(std::size_t N, std::size_t limit)
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;

        for (; elapsed < limit; ++count)
        {
            Subject subject;
            std::vector<Foo> foo(N);

            for (auto& foo_instance : foo)
            {
                Foo::connect_method(subject, foo_instance);
            }
            s_timer.reset();

            Foo::emit_method(subject, s_rng);

            elapsed += s_timer.count<Timer_u>();
        }
        return calculate_score(N, elapsed, count);
    }

    //--------------------------------------------------------------------------

    static double combined(std::size_t N, std::size_t limit)
    {
        std::size_t count = 1;
        std::size_t elapsed = 0;

        s_timer.reset();

        Subject subject;

        for (; elapsed < limit; ++count, elapsed = s_timer.count<Timer_u>())
        {
            std::vector<Foo> foo(N);

            for (auto& foo_instance : foo)
            {
                Foo::connect_method(subject, foo_instance);
            }
            Foo::emit_method(subject, s_rng);
        }
        return calculate_score(N, elapsed, count);
    }

    //--------------------------------------------------------------------------

    static double threaded(std::size_t N, std::size_t limit)
    {
        Subject subject;

        auto context = [&]()
        {
            std::size_t count = 1;
            std::size_t elapsed = 0;

            Rng rng(s_rng);
            ChronoTimer timer;

            for (; elapsed < limit; ++count, elapsed = timer.count<Timer_u>())
            {
                std::vector<Foo> foo(N);

                for (auto& foo_instance : foo)
                {
                    Foo::connect_method(subject, foo_instance);
                }
                Foo::emit_method(subject, rng);
            }
            return calculate_score(N, elapsed, count);
        };

        std::vector<double> results;
        std::vector<std::future<double>> future_results;
        for (std::size_t i = std::thread::hardware_concurrency() / 2; i > 0; --i)
        {
            future_results.emplace_back(std::async(std::launch::async, context));
        }
        for (auto& future_result : future_results)
        {
            results.emplace_back(future_result.get());
        }
        return std::accumulate(results.begin(), results.end(), 0.0) / results.size();
    }
};

//------------------------------------------------------------------------------

template <typename Subject, typename Foo>
ChronoTimer Benchmark<Subject, Foo>::s_timer;

template <typename Subject, typename Foo>
Rng Benchmark<Subject, Foo>::s_rng;
