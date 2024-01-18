#include "ExecuteAwaiter.hpp"
namespace app_helpers::execute_awaiter
{
    // functin to call lambda
    struct app_helpers::execute_awaiter::ExecuteAwaiter executeIntensiveFunction(std::function<void()> func)
    {
        struct app_helpers::execute_awaiter::ExecuteAwaiter retValue
        {
            std::move(func)
        };
        return retValue;
    }
    // function to call emtry lambda and return execution in the thread from threads pool
    struct app_helpers::execute_awaiter::ExecuteAwaiter switchToThreadPull()
    {
        const std::function<void()> &func = []()
        { return; };
        struct app_helpers::execute_awaiter::ExecuteAwaiter retValue
        {
            std::move(func)
        };
        return retValue;
    }
}
