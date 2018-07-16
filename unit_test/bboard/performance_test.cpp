#include <cmath>
#include <thread>
#include <chrono>
#include <utility>
#include <iostream>

#include "catch.hpp"
#include "testing_utilities.hpp"

#include "bboard.hpp"
#include "agents.hpp"
#include "colors.hpp"

using bboard::FixedQueue;

template<typename F, typename... Args>
double timeMethod(int times, F func, Args&&... args)
{
    std::chrono::duration<double, std::milli> total;
    auto t1 = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < times; i++)
    {
        func(std::forward<Args>(args)...);
    }
    total = std::chrono::high_resolution_clock::now() - t1;
    return total.count();
}

void Proxy(bboard::Environment& s)
{
    s.Step();
}

void ProxyConcurrent(int times)
{
    agents::HarmlessAgent a;
    bboard::Environment env;
    env.MakeGame({&a, &a, &a, &a});
    for(int i = 0; i < times; i++)
    {
        env.Step();
    }
}

TEST_CASE("Step Function", "[performance]")
{
    agents::HarmlessAgent a;
    bboard::Environment env;

    env.MakeGame({&a, &a, &a, &a});

    int times = 100000;

    double t = -1;

    if(!THREADING)
    {
        t = timeMethod(times, Proxy, env);
    }
    else
    {
        std::vector<std::thread> threads(THREAD_COUNT);

        std::chrono::duration<double, std::milli> total;
        auto t1 = std::chrono::high_resolution_clock::now();
        for(uint i = 0; i < THREAD_COUNT; i++)
        {
            threads[i] = std::thread(ProxyConcurrent, times);
        }
        // join all
        for(uint i = 0; i < THREAD_COUNT; i++)
        {
            threads[i].join();
        }
        total = std::chrono::high_resolution_clock::now() - t1;
        t = total.count();
    }


    std::cout << std::endl
              << FGRN("Test Results:") << std::endl
              << "Iteration count (100ms):         ";
    if(THREADING)
        std::cout << FormatWithCommas(uint(std::floor(times/(t/100.0))) * THREAD_COUNT);
    else
        std::cout << FormatWithCommas(uint(std::floor(times/(t/100.0))));
    std::cout << std::endl
              << "Tested with:                     "
              << type_name<decltype(a)>()
              << std::endl << std::endl;

    REQUIRE(1);
}
