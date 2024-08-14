#include <algorithm>
#include <cctype>
#include <coroutine>
#include <iostream>
#include <utility>

using namespace std::literals;

struct Task {
    struct promise_type {
        std::string data_in_{};  // storing values for or from the coroutine
        std::string data_out_{};

        void unhandled_exception() noexcept { }  // how to handle exceptions
        Task get_return_object() { return Task{*this}; }  // create coroutine wrapper object
        std::suspend_always initial_suspend() noexcept { return {}; }  // coroutine startup
        std::suspend_always yield_value(std::string msg) noexcept  // value from co_yield
        {
            data_out_ = std::move(msg);
            return {};
        }

        auto await_transform(std::string) noexcept  // value from co_await
        {
            struct awaiter {  // customized version instead of using std::suspend_always or std::suspend_never
                promise_type& pt;
                constexpr bool await_ready() const noexcept { return true; }
                void await_suspend(std::coroutine_handle<>) const noexcept { }
                std::string await_resume() const noexcept { return std::move(pt.data_in_); }
            };

            return awaiter{*this};
        }

        void return_value(std::string msg) noexcept { data_out_ = std::move(msg); }  // value from co_return
        std::suspend_always final_suspend() noexcept { return {}; }  // ending the coroutine
    };

    std::coroutine_handle<promise_type> handle_{};

    explicit Task(promise_type& p) : handle_{std::coroutine_handle<promise_type>::from_promise(p)} { }  // get coroutine handle from the promise
    Task(Task&& rhs) noexcept : handle_{std::exchange(rhs.handle_, nullptr)} { }  // move only

    ~Task() noexcept
    {
        if (handle_)
            handle_.destroy();  // free coroutine memory
    }

    std::string listen()
    {
        if (!handle_.done())
            handle_.resume();

        return std::move(handle_.promise().data_out_);
    }

    void answer(std::string msg)
    {
        handle_.promise().data_in_ = std::move(msg);

        if (!handle_.done())
            handle_.resume();
    }
};

Task uppercase()
{
    co_yield "Hello, how may I help you today?\n"s;

    auto input = co_await std::string();

    std::cout << input;
    std::cout << "Sure thing, here we go...\n";

    std::transform(input.begin(), input.end(), input.begin(), [](const unsigned char c) { return static_cast<unsigned char>(std::toupper(c)); });
    co_return input;
}

int main()
{
    Task to_upper = uppercase();
    std::cout << to_upper.listen();
    to_upper.answer("transform this text to uppercase, please!\n"s);
    std::cout << to_upper.listen();
}
