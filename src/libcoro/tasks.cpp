#include <cassert>

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "coro/coro.hpp"

#include "fibonacci.hpp"
#include "print_and_assert_result.hpp"

coro::task<uint64_t> fibonacci_coro(const int nth) { co_return fibonacci(nth); }

void run_simple_tasks()
{
    const auto double_coro = [](const int n) -> coro::task<int> { co_return n * 2; };
    const auto double_100_and_add_coro = [&](const int n) -> coro::task<int> { co_return co_await double_coro(100) + n; };

    print_and_assert_result("double_coro", 400, coro::sync_wait(double_coro(200)));
    print_and_assert_result("double_100_and_add_coro", 205, coro::sync_wait(double_100_and_add_coro(5)));
}

void run_fibonacci()
{
    print_and_assert_result<uint64_t>("fibonacci_coro(10)", 55, coro::sync_wait(fibonacci_coro(10)));
    print_and_assert_result<uint64_t>("fibonacci_coro(20)", 6765, coro::sync_wait(fibonacci_coro(20)));
}

void benchmark()
{
    ankerl::nanobench::Bench().run("libcoro: simple_tasks", [] {
        const auto double_coro = [](const int n) -> coro::task<int> { co_return n * 2; };
        const auto double_100_and_add_coro = [&](const int n) -> coro::task<int> { co_return co_await double_coro(100) + n; };

        const auto a = coro::sync_wait(double_coro(200));
        const auto b = coro::sync_wait(double_100_and_add_coro(5));
        const auto result = a + b;
        ankerl::nanobench::doNotOptimizeAway(result);
        assert(result == 605);
    });

    ankerl::nanobench::Bench().run("libcoro: fibonacci", [] {
        const auto result = coro::sync_wait(fibonacci_coro(20));
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
