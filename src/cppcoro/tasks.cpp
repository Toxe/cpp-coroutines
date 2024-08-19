#include <cassert>

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "cppcoro/sync_wait.hpp"
#include "cppcoro/task.hpp"

#include "fibonacci.hpp"
#include "print_and_assert_result.hpp"

cppcoro::task<uint64_t> fibonacci_coro(const int nth) { co_return fibonacci(nth); }

void run_simple_tasks()
{
    const auto double_coro = [](const int n) -> cppcoro::task<int> { co_return n * 2; };
    const auto double_100_and_add_coro = [&](const int n) -> cppcoro::task<int> { co_return co_await double_coro(100) + n; };

    print_and_assert_result("double_coro", 400, cppcoro::sync_wait(double_coro(200)));
    print_and_assert_result("double_100_and_add_coro", 205, cppcoro::sync_wait(double_100_and_add_coro(5)));
}

void run_fibonacci()
{
    print_and_assert_result<uint64_t>("fibonacci_coro(10)", 55, cppcoro::sync_wait(fibonacci_coro(10)));
    print_and_assert_result<uint64_t>("fibonacci_coro(20)", 6765, cppcoro::sync_wait(fibonacci_coro(20)));
}

void benchmark()
{
    ankerl::nanobench::Bench().run("cppcoro: simple_tasks", [] {
        const auto double_coro = [](const int n) -> cppcoro::task<int> { co_return n * 2; };
        const auto double_100_and_add_coro = [&](const int n) -> cppcoro::task<int> { co_return co_await double_coro(100) + n; };

        const auto a = cppcoro::sync_wait(double_coro(200));
        const auto b = cppcoro::sync_wait(double_100_and_add_coro(5));
        const auto result = a + b;
        ankerl::nanobench::doNotOptimizeAway(result);
        assert(result == 605);
    });

    ankerl::nanobench::Bench().run("cppcoro: fibonacci", [] {
        const auto result = cppcoro::sync_wait(fibonacci_coro(20));
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
