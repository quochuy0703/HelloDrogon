#pragma once

#include <iostream>
#include <drogon/drogon.h>
#include <map>

namespace app_helpers::fetch_helper
{
    template <typename T>
    class IFetch
    {
        virtual T Get(const std::string &url, const std::map<std::string, std::string> &queries = {},
                      const std::string &endpoint = "") = 0;

        virtual T Post(const std::string &url, Json::Value body = Json::Value(),
                       const std::string &endpoint = "") = 0;

        virtual T Put(const std::string &url, Json::Value body = Json::Value(),
                      const std::string &endpoint = "") = 0;

        virtual T Delete(const std::string &url,
                         const std::string &endpoint = "") = 0;
    };
}