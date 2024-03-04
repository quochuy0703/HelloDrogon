#pragma once

#include <iostream>
#include "IFetch.cpp"
#include <drogon/drogon.h>
#include "../utils/ApiResponse.hpp"

using namespace app_helpers::api_res_helper;

namespace app_helpers::fetch_helper
{

    class Fetch : public IFetch<drogon::Task<ApiResponse<Json::Value>>>
    {
    public:
        drogon::Task<ApiResponse<Json::Value>> Get(const std::string &url, const std::map<std::string, std::string> &queries = {},
                                                   const std::string &endpoint = "") override;
        drogon::Task<ApiResponse<Json::Value>> Post(const std::string &url, Json::Value body = Json::Value(),
                                                    const std::string &endpoint = "") override;
        drogon::Task<ApiResponse<Json::Value>> Put(const std::string &url, Json::Value body = Json::Value(),
                                                   const std::string &endpoint = "") override;
        drogon::Task<ApiResponse<Json::Value>> Delete(const std::string &url,
                                                      const std::string &endpoint = "") override;
    };
}