#include "fibonacci.hpp"

#include <utility>

uint64_t fibonacci(const int nth)
{
    uint64_t a = 0;
    uint64_t b = 1;

    for (int i = 0; i < nth; ++i)
        a = std::exchange(b, a + b);

    return a;
}
