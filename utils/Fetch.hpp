#pragma once

#include "IFetch.cpp"
#include <drogon/HttpController.h>
#include "../utils/ApiResponse.hpp"

using namespace app_helpers::api_res_helper;

namespace app_helpers::fetch_helper
{

    class Fetch : public IFetch<ApiResponse<Json::Value>>
    {
    public:
        ApiResponse<Json::Value> Get(const std::string &url, const std::map<std::string, std::string> &queries = {},
                                     const std::string &endpoint = "") override;
        ApiResponse<Json::Value> Delete(const std::string &url, const std::map<std::string, std::string> &queries = {},
                                        const std::string &endpoint = "") override;
    };
}