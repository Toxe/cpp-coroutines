#include <cassert>

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "cppcoro/generator.hpp"

#include "print_and_assert_result.hpp"

cppcoro::generator<int> infinite_counter()
{
    for (int i = 1;; ++i)
        co_yield i;
}

cppcoro::generator<int> finite_counter(const int max)
{
    assert(max >= 1);

    for (int i = 1; i <= max; ++i)
        co_yield i;
}

void run_infinite_counter()
{
    int sum = 0;

    for (const auto i : infinite_counter()) {
        sum += i;

        if (i == 1000)
            break;
    }

    print_and_assert_result("infinite_counter", 500500, sum);
}

void run_finite_counter_iterator()
{
    auto gen = finite_counter(1000);
    int sum = 0;

    for (auto it = gen.begin(); it != gen.end(); ++it)
        sum += *it;

    print_and_assert_result("finite_counter_iterator", 500500, sum);
}

void run_finite_counter_range_for()
{
    int sum = 0;

    for (const auto i : finite_counter(1000))
        sum += i;

    print_and_assert_result("finite_counter_range_for", 500500, sum);
}

void benchmark()
{
    ankerl::nanobench::Bench().run("cppcoro generator: infinite_counter", [] {
        int sum = 0;

        for (const auto i : infinite_counter()) {
            sum += i;

            if (i == 1000)
                break;
        }

        ankerl::nanobench::doNotOptimizeAway(sum);
        assert(sum == 500500);
    });

    ankerl::nanobench::Bench().run("cppcoro generator: finite_counter (iterator)", [] {
        auto gen = finite_counter(1000);
        int sum = 0;

        for (auto it = gen.begin(); it != gen.end(); ++it)
            sum += *it;

        ankerl::nanobench::doNotOptimizeAway(sum);
        assert(sum == 500500);
    });

    ankerl::nanobench::Bench().run("cppcoro generator: finite_counter (range-for)", [] {
        int sum = 0;

        for (const auto i : finite_counter(1000))
            sum += i;

        ankerl::nanobench::doNotOptimizeAway(sum);
        assert(sum == 500500);
    });
}

int main()
{
    run_infinite_counter();
    run_finite_counter_iterator();
    run_finite_counter_range_for();

    benchmark();
}
