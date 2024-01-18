#include <coroutine>
#include <iostream>
#include <optional>
#include <string_view>
#include <thread>
#include <vector>
#include <chrono>

using namespace std::chrono;

std::thread *threadg;

namespace app_helpers
{
    namespace coro_helper
    {

        template <typename T>
        struct future
        {
            struct promise_type
            {
                T value;
                future get_return_object()
                {
                    return {std::coroutine_handle<promise_type>::from_promise(*this)};
                }
                std::suspend_always initial_suspend() noexcept
                {
                    std::cout << "initial" << std::endl;
                    return {};
                }
                std::suspend_always final_suspend() noexcept
                {
                    std::cout << "final" << std::endl;
                    return {};
                }
                void return_value(T x)
                {
                    std::cout << "return value" << std::endl;
                    value = std::move(x);
                }
                void unhandled_exception() noexcept {}

                ~promise_type() { std::cout << "future ~promise_type" << std::endl; }
            };

            struct AwaitableFuture
            {
                future &m_future;
                bool await_ready() const noexcept { return false; }

                void await_suspend(std::coroutine_handle<> handle)
                {
                    std::cout << "await_suspend" << std::endl;

                    *threadg = std::thread([this, handle]
                                           {
        std::cout << "Launch thread: " << std::this_thread::get_id()
                  << std::endl;
        m_future.coro.resume();
        handle.resume(); });
                }

                T await_resume()
                {
                    std::cout << "await_resume" << std::endl;
                    return m_future.coro.promise().value;
                }

                ~AwaitableFuture() { std::cout << "~AwaitableFuture" << std::endl; }
            };

            std::coroutine_handle<promise_type> coro;

            future(std::coroutine_handle<promise_type> coro) : coro{coro} {}

            ~future()
            {
                std::cout << "~future" << std::endl;
                if (coro)
                    coro.destroy();
            }

            AwaitableFuture operator co_await()
            {
                std::cout << "co_await" << std::endl;
                return {*this};
            }
        };

        template <typename F, typename... Args>
        future<std::invoke_result_t<F, Args...>> async(F f, Args... args)
        {
            std::cout << "async" << std::endl;
            co_return f(args...);
        }

        int square(int x)
        {

            std::cout << "square in thread id " << std::this_thread::get_id()
                      << std::endl;
            // std::this_thread::sleep_for(seconds(3));
            return x * x;
        }

    }
}
