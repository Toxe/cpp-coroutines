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
    T await_resume() const noexcept { return result(); }

    T result() const
    {
        if (!handle_.done())
            handle_.resume();

        return handle_.promise().result_;
    }
};

Task<uint64_t> fibonacci_coro(const int nth) { co_return fibonacci(nth); }

void run_simple_tasks()
{
    const auto double_coro = [](const int n) -> Task<int> { co_return n * 2; };
    const auto double_100_and_add_coro = [&](const int n) -> Task<int> { co_return co_await double_coro(100) + n; };

    print_and_assert_result("double_coro", 400, double_coro(200).result());
    print_and_assert_result("double_100_and_add_coro", 205, double_100_and_add_coro(5).result());
}

void run_fibonacci()
{
    print_and_assert_result<uint64_t>("fibonacci_coro(10)", 55, fibonacci_coro(10).result());
    print_and_assert_result<uint64_t>("fibonacci_coro(20)", 6765, fibonacci_coro(20).result());
}

void benchmark()
{
    ankerl::nanobench::Bench().run("manual: simple_tasks", [&] {
        const auto double_coro = [](const int n) -> Task<int> { co_return n * 2; };
        const auto double_100_and_add_coro = [&](const int n) -> Task<int> { co_return co_await double_coro(100) + n; };

        const auto a = double_coro(200).result();
        const auto b = double_100_and_add_coro(5).result();
        const auto result = a + b;
        ankerl::nanobench::doNotOptimizeAway(result);
        assert(result == 605);
    });

    ankerl::nanobench::Bench().run("manual: fibonacci", [&] {
        const auto result = fibonacci_coro(20).result();
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
