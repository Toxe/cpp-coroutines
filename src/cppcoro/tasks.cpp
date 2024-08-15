#include <cassert>

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "cppcoro/sync_wait.hpp"
#include "cppcoro/task.hpp"

#include "fibonacci.hpp"
#include "print_and_assert_result.hpp"

cppcoro::task<uint64_t> fibonacci_coro(const int nth) { co_return fibonacci(nth); }

void run_fibonacci()
{
    // create new tasks but don't start executing the coroutines yet
    const cppcoro::task<uint64_t> fibonacci10_task = fibonacci_coro(10);
    const cppcoro::task<uint64_t> fibonacci20_task = fibonacci_coro(20);

    // execute the coroutines
    print_and_assert_result<uint64_t>("fibonacci(10)", 55, cppcoro::sync_wait(fibonacci10_task));
    print_and_assert_result<uint64_t>("fibonacci(20)", 6765, cppcoro::sync_wait(fibonacci20_task));
}

void run_simple_tasks()
{
    // create new tasks but don't start executing the coroutines yet
    const auto make_double_task = [](const int n) -> cppcoro::task<int> { co_return n * 2; };
    const auto double100_task = make_double_task(100);

    const auto make_double100_and_add_task = [&](const int n) -> cppcoro::task<int> { co_return co_await double100_task + n; };
    const auto double100_and_add_five_task = make_double100_and_add_task(5);

    // execute the coroutines
    print_and_assert_result("double100_task", 200, cppcoro::sync_wait(double100_task));
    print_and_assert_result("double100_and_add_five_task", 205, cppcoro::sync_wait(double100_and_add_five_task));
}

void benchmark()
{
    const auto double100_task = [](const int n) -> cppcoro::task<int> { co_return n * 2; }(100);
    const auto double100_and_add_five_task = [&](const int n) -> cppcoro::task<int> { co_return co_await double100_task + n; }(5);

    ankerl::nanobench::Bench().run("cppcoro: simple_tasks", [&] {
        const auto a = cppcoro::sync_wait(double100_task);
        const auto b = cppcoro::sync_wait(double100_and_add_five_task);
        const auto result = a + b;
        ankerl::nanobench::doNotOptimizeAway(result);
        assert(result == 405);
    });

    const cppcoro::task<uint64_t> fibonacci_task = fibonacci_coro(20);

    ankerl::nanobench::Bench().run("cppcoro: fibonacci", [&] {
        const auto result = cppcoro::sync_wait(fibonacci_task);
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
