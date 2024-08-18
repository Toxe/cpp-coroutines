#include <cassert>
#include <coroutine>

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "fibonacci.hpp"
#include "print_and_assert_result.hpp"

template <typename T>
struct Task {
    struct promise_type {
        T result_{};

        Task get_return_object() { return Task{*this}; }
        std::suspend_always initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        void return_value(T value) noexcept { result_ = value; }
        void unhandled_exception() { }
    };

    std::coroutine_handle<promise_type> handle_{};

    explicit Task(promise_type& p) : handle_{std::coroutine_handle<promise_type>::from_promise(p)} { }
    Task(Task&& rhs) noexcept : handle_{std::exchange(rhs.handle_, nullptr)} { }  // move only

    ~Task() noexcept
    {
        if (handle_)
            handle_.destroy();  // free coroutine memory
    }

    // Awaiter interface
    bool await_ready() const noexcept { return true; }
    void await_suspend(std::coroutine_handle<>) const noexcept { }
    T await_resume() const noexcept { return handle_.promise().result_; }

    T result()
    {
        if (!handle_.done())
            handle_.resume();

        return handle_.promise().result_;
    }
};

Task<uint64_t> fibonacci_coro(const int nth) { co_return fibonacci(nth); }

void run_fibonacci()
{
    // create new tasks but don't start executing the coroutines yet
    Task<uint64_t> fibonacci10_task = fibonacci_coro(10);
    Task<uint64_t> fibonacci20_task = fibonacci_coro(20);

    // execute the coroutines
    print_and_assert_result<uint64_t>("fibonacci(10)", 55, fibonacci10_task.result());
    print_and_assert_result<uint64_t>("fibonacci(20)", 6765, fibonacci20_task.result());
}

void run_simple_tasks()
{
    // create new tasks but don't start executing the coroutines yet
    auto make_double_task = [](const int n) -> Task<int> { co_return n * 2; };
    auto double100_task = make_double_task(100);

    auto make_double100_and_add_task = [&](const int n) -> Task<int> { co_return co_await double100_task + n; };
    auto double100_and_add_five_task = make_double100_and_add_task(5);

    // execute the coroutines
    print_and_assert_result("double100_task", 200, double100_task.result());
    print_and_assert_result("double100_and_add_five_task", 205, double100_and_add_five_task.result());
}

void benchmark()
{
    ankerl::nanobench::Bench().run("manual: simple_tasks", [&] {
        auto double100_task = [](const int n) -> Task<int> { co_return n * 2; }(100);
        auto double100_and_add_five_task = [&](const int n) -> Task<int> { co_return co_await double100_task + n; }(5);

        const auto a = double100_task.result();
        const auto b = double100_and_add_five_task.result();
        const auto result = a + b;
        ankerl::nanobench::doNotOptimizeAway(result);
        assert(result == 405);
    });

    ankerl::nanobench::Bench().run("manual: fibonacci", [&] {
        Task<uint64_t> fibonacci_task = fibonacci_coro(20);

        const auto result = fibonacci_task.result();
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
