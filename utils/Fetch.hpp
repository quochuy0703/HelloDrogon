#pragma once

#include "IFetch.cpp"
#include <drogon/HttpController.h>
#include "../utils/ApiResponse.hpp"

using namespace app_helpers::api_res_helper;

namespace app_helpers::fetch_helper
{
    template <typename T>
    class Fetch : public IFetch<T>
    {
    public:
        T Get(const std::string &url, const std::map<std::string, std::string> &queries = {},
              const std::string &endpoint = "") override;
    };
}