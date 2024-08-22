#pragma once

#include <cassert>
#include <iostream>
#include <string_view>

template <typename T>
#if NDEBUG
void print_and_assert_result(const std::string_view label, const T, const T result)
#else
void print_and_assert_result(const std::string_view label, const T expected, const T result)
#endif
{
    assert(result == expected);
    std::cout << label << ": " << result << '\n';
}
