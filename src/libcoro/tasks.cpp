#include <cassert>

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "coro/coro.hpp"

#include "fibonacci.hpp"
#include "print_and_assert_result.hpp"

coro::task<uint64_t> fibonacci_coro(const int nth) { co_return fibonacci(nth); }

void run_fibonacci()
{
    // create new tasks but don't start executing the coroutines yet
    const coro::task<uint64_t> fibonacci10_task = fibonacci_coro(10);
    const coro::task<uint64_t> fibonacci20_task = fibonacci_coro(20);

    // execute the coroutines
    print_and_assert_result<uint64_t>("fibonacci(10)", 55, coro::sync_wait(fibonacci10_task));
    print_and_assert_result<uint64_t>("fibonacci(20)", 6765, coro::sync_wait(fibonacci20_task));
}

void run_simple_tasks()
{
    // create new tasks but don't start executing the coroutines yet
    const auto make_double_task = [](const int n) -> coro::task<int> { co_return n * 2; };
    const auto double100_task = make_double_task(100);

    const auto make_double100_and_add_task = [&](const int n) -> coro::task<int> { co_return co_await double100_task + n; };
    const auto double100_and_add_five_task = make_double100_and_add_task(5);

    // execute the coroutines
    print_and_assert_result("double100_task", 200, coro::sync_wait(double100_task));
    print_and_assert_result("double100_and_add_five_task", 205, coro::sync_wait(double100_and_add_five_task));
}

void benchmark()
{
    ankerl::nanobench::Bench().run("libcoro: simple_tasks", [&] {
        const auto double100_task = [](const int n) -> coro::task<int> { co_return n * 2; }(100);
        const auto double100_and_add_five_task = [&](const int n) -> coro::task<int> { co_return co_await double100_task + n; }(5);

        const auto a = coro::sync_wait(double100_task);
        const auto b = coro::sync_wait(double100_and_add_five_task);
        const auto result = a + b;
        ankerl::nanobench::doNotOptimizeAway(result);
        assert(result == 405);
    });

    ankerl::nanobench::Bench().run("libcoro: fibonacci", [&] {
        const coro::task<uint64_t> fibonacci_task = fibonacci_coro(20);

        const auto result = coro::sync_wait(fibonacci_task);
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
