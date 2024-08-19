#include <cassert>

#define ANKERL_NANOBENCH_IMPLEMENT
#include "nanobench.h"

#include "print_and_assert_result.hpp"

class InfiniteCounter {
public:
    [[nodiscard]] int next() { return ++value_; }

private:
    int value_ = 0;
};

class FiniteCounter {
public:
    FiniteCounter(const int max) : max_{max} { }

    [[nodiscard]] int next() { return ++value_; }
    [[nodiscard]] bool done() { return value_ >= max_; }

private:
    int max_;
    int value_ = 0;
};

void run_infinite_counter()
{
    InfiniteCounter gen;
    int sum = 0;
    int i;

    while ((i = gen.next()) <= 1000)
        sum += i;

    print_and_assert_result("infinite_counter", 500500, sum);
}

void run_finite_counter()
{
    FiniteCounter gen(1000);
    int sum = 0;

    while (!gen.done())
        sum += gen.next();

    print_and_assert_result("finite_counter", 500500, sum);
}

void benchmark()
{
    ankerl::nanobench::Bench().run("reference generator: infinite_counter", [] {
        InfiniteCounter gen;
        int sum = 0;
        int i;

        while ((i = gen.next()) <= 1000)
            sum += i;

        ankerl::nanobench::doNotOptimizeAway(sum);
        assert(sum == 500500);
    });

    ankerl::nanobench::Bench().run("reference generator: finite_counter", [] {
        FiniteCounter gen(1000);
        int sum = 0;

        while (!gen.done())
            sum += gen.next();

        ankerl::nanobench::doNotOptimizeAway(sum);
        assert(sum == 500500);
    });
}

int main()
{
    run_infinite_counter();
    run_finite_counter();

    benchmark();
}
