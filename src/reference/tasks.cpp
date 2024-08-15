#include <cassert>
#include <iostream>
#include <string_view>
#include <utility>

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

void print_and_assert_result(const std::string_view label, const uint64_t expected, const uint64_t result)
{
    assert(result == expected);
    std::cout << label << ": " << result << '\n';
}

uint64_t fibonacci(const int nth)
{
    uint64_t a = 0;
    uint64_t b = 1;

    for (int i = 0; i < nth; ++i)
        a = std::exchange(b, a + b);

    return a;
}

void run_fibonacci()
{
    print_and_assert_result("fibonacci(10)", 55, fibonacci(10));
    print_and_assert_result("fibonacci(20)", 6765, fibonacci(20));
}

void run_simple_tasks()
{
    auto double_task = [](int n) -> int { return n * 2; };
    auto double_and_add_five_task = [&](int n) -> int { return double_task(100) + n; };

    print_and_assert_result("double_task", 200, double_task(100));
    print_and_assert_result("double_and_add_five_task", 205, double_and_add_five_task(5));
}

void benchmark()
{
    auto double_task = [](int n) -> int { return n * 2; };
    auto double_and_add_five_task = [&](int n) -> int { return double_task(100) + n; };

    ankerl::nanobench::Bench().run("reference: simple_tasks", [&] {
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
