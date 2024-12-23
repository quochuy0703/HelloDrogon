#pragma once

#include <iostream>
#include <map>
#include <string>
#include "../utils/Fetch.hpp"

using namespace app_helpers::fetch_helper;

namespace app_services
{
    class FakeApiService
    {
    public:
        FakeApiService()
        {
            LOG_INFO << "Init FakeApiService";
            fetch = std::make_shared<Fetch>(BaseURL);
        }
        ~FakeApiService()
        {
            LOG_INFO << "Destructor FakeApiService";
        }

        drogon::Task<Json::Value> searchProduct(const std::map<std::string, std::string> &queries = {});

    private:
        const std::string BaseURL = "https://dummyjson.com";
        std::shared_ptr<Fetch> fetch;
    };

}