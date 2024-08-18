#include <cassert>

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "fibonacci.hpp"
#include "print_and_assert_result.hpp"

void run_fibonacci()
{
    print_and_assert_result<uint64_t>("fibonacci(10)", 55, fibonacci(10));
    print_and_assert_result<uint64_t>("fibonacci(20)", 6765, fibonacci(20));
}

void run_simple_tasks()
{
    const auto double_task = [](int n) -> int { return n * 2; };
    const auto double_and_add_five_task = [&](int n) -> int { return double_task(100) + n; };

    print_and_assert_result("double_task", 200, double_task(100));
    print_and_assert_result("double_and_add_five_task", 205, double_and_add_five_task(5));
}

void benchmark()
{
    ankerl::nanobench::Bench().run("reference: simple_tasks", [&] {
        const auto double_task = [](int n) -> int { return n * 2; };
        const auto double_and_add_five_task = [&](int n) -> int { return double_task(100) + n; };

        const auto a = double_task(100);
        const auto b = double_and_add_five_task(5);
        const auto result = a + b;
        ankerl::nanobench::doNotOptimizeAway(result);
        assert(result == 405);
    });

    ankerl::nanobench::Bench().run("reference: fibonacci", [] {
        const auto result = fibonacci(20);
        ankerl::nanobench::doNotOptimizeAway(result);
        assert(result == 6765);
    });
}

int main()
{
    run_simple_tasks();
    run_fibonacci();

    benchmark();
}
