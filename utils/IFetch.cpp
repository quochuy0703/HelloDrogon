#pragma once

#include <iostream>
#include <map>

namespace app_helpers::fetch_helper
{
    template <typename T>
    class IFetch
    {
        virtual T Get(const std::string &url, const std::map<std::string, std::string> &queries = {},
                      const std::string &endpoint = "") = 0;

        // virtual T Post(std::string route, object data) = 0;

        // virtual T Put(std::string route, object data) = 0;

        virtual T Delete(const std::string &url, const std::map<std::string, std::string> &queries = {},
                         const std::string &endpoint = "") = 0;
    };
}