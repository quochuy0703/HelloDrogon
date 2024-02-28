#pragma once

#include <trantor/utils/ConcurrentTaskQueue.h>
#include <drogon/drogon.h>
using namespace drogon;
namespace app_helpers::execute_awaiter
{
    // define thread pool
    static trantor::ConcurrentTaskQueue globalThreadPool(10, "h2load test");
    // declare awaitable for coroutine (my definitions to make co_await from coroutine)
    using calcIntensive = std::function<void()>;
    struct [[nodiscard]] ExecuteAwaiter : public CallbackAwaiter<void>
    {
        explicit ExecuteAwaiter(calcIntensive func)
            : callAndResume_{std::move(func)}
        {
        }
        void await_suspend(std::coroutine_handle<> handle)
        {
            auto taskToQueue = [this, handle]()
            {
                try
                {
                    this->callAndResume_();
                    handle.resume();
                }
                catch (...)
                {
                    auto eptr = std::current_exception();
                    setException(eptr);
                    handle.resume();
                }
            };
            globalThreadPool.runTaskInQueue(taskToQueue);
        };

    private:
        calcIntensive callAndResume_;
    };
    // functin to call lambda
    struct ExecuteAwaiter executeIntensiveFunction(std::function<void()> func);
    // function to call emtry lambda and return execution in the thread from threads pool
    struct ExecuteAwaiter switchToThreadPull();

    template <typename R>
    using calcIntensiveReturn = std::function<R()>;

    template <typename R>
    struct [[nodiscard]] ExecuteAwaiterReturn : public CallbackAwaiter<R>
    {
        explicit ExecuteAwaiterReturn(calcIntensiveReturn<R> func)
            : callAndResume_{std::move(func)}
        {
        }
        void await_suspend(std::coroutine_handle<> handle)
        {
            auto taskToQueue = [this, handle]()
            {
                try
                {
                    R value = this->callAndResume_();
                    this->setValue(value);
                    handle.resume();
                }
                catch (...)
                {
                    auto eptr = std::current_exception();
                    this->setException(eptr);
                    handle.resume();
                }
            };
            globalThreadPool.runTaskInQueue(taskToQueue);
        };

    private:
        calcIntensiveReturn<R> callAndResume_;
    };

    // functin to call lambda
    template <typename R>
    struct ExecuteAwaiterReturn<R> executeIntensiveFunctionReturn(std::function<R()> func)
    {
        struct app_helpers::execute_awaiter::ExecuteAwaiterReturn<R> retValue
        {
            std::move(func)
        };
        return retValue;
    }

}
