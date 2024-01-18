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

}
