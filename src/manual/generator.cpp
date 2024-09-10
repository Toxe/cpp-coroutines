#include <cassert>
#include <coroutine>

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "print_and_assert_result.hpp"

template <typename T>
struct Generator {
    struct promise_type {
        T value_{};

        Generator get_return_object() { return Generator{*this}; }
        void unhandled_exception() { }
        void return_void() noexcept { }
        std::suspend_never initial_suspend() noexcept { return {}; }
        std::suspend_always final_suspend() noexcept { return {}; }
        std::suspend_always yield_value(T value) noexcept
        {
            value_ = value;
            return {};
        }
    };

    struct iterator {
        std::coroutine_handle<promise_type> handle_{};

        bool operator==(std::default_sentinel_t) const { return handle_.done(); }
        int operator*() const { return handle_.promise().value_; }

        iterator& operator++()
        {
            handle_.resume();
            return *this;
        }
    };

    std::coroutine_handle<promise_type> handle_{};

    explicit Generator(promise_type& p) : handle_{std::coroutine_handle<promise_type>::from_promise(p)} { }
    Generator(Generator&& rhs) noexcept : handle_{std::exchange(rhs.handle_, nullptr)} { }  // move only

    ~Generator() noexcept
    {
        if (handle_)
            handle_.destroy();  // free coroutine memory
    }

    T value() const { return handle_.promise().value_; }
    bool done() const { return handle_.done(); }
    void resume() const { return handle_.resume(); }

    iterator begin() const { return {handle_}; }
    std::default_sentinel_t end() const { return {}; }
};

Generator<int> infinite_counter()
{
    for (int i = 1;; ++i)
        co_yield i;
}

Generator<int> finite_counter(const int max)
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

void run_finite_counter()
{
    auto gen = finite_counter(1000);
    int sum = 0;

    while (!gen.done()) {
        sum += gen.value();
        gen.resume();
    }

    print_and_assert_result("finite_counter", 500500, sum);
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
    ankerl::nanobench::Bench().run("manual generator: infinite_counter", [] {
        int sum = 0;

        for (const auto i : infinite_counter()) {
            sum += i;

            if (i == 1000)
                break;
        }

        ankerl::nanobench::doNotOptimizeAway(sum);
        assert(sum == 500500);
    });

    ankerl::nanobench::Bench().run("manual generator: finite_counter", [] {
        auto gen = finite_counter(1000);
        int sum = 0;

        while (!gen.done()) {
            sum += gen.value();
            gen.resume();
        }

        ankerl::nanobench::doNotOptimizeAway(sum);
        assert(sum == 500500);
    });

    ankerl::nanobench::Bench().run("manual generator: finite_counter (iterator)", [] {
        auto gen = finite_counter(1000);
        int sum = 0;

        for (auto it = gen.begin(); it != gen.end(); ++it)
            sum += *it;

        ankerl::nanobench::doNotOptimizeAway(sum);
        assert(sum == 500500);
    });

    ankerl::nanobench::Bench().run("manual generator: finite_counter (range-for)", [] {
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
    run_finite_counter();
    run_finite_counter_iterator();
    run_finite_counter_range_for();

    benchmark();
}
