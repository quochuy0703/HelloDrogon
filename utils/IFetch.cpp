#include <iostream>

namespace app_helpers::fetch_helper
{
    template <typename T>
    class IFetch
    {
        virtual T Get<T>(std::string route) = 0;

        virtual T Post<T>(std::string route, object data) = 0;

        virtual T Put<T>(std::string route, object data) = 0;

        virtual T Delete<T>(std::string route) = 0;
    };
}