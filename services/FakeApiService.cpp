#include "FakeApiService.hpp"

// map<std::string, std::string> query = {{"q", "phone"}};
// ApiResponse<Json::Value> result = co_await fetch.Get("/products/search", query, "https://dummyjson.com");

drogon::Task<Json::Value> app_services::FakeApiService::searchProduct(const std::map<std::string, std::string> &queries)
{
    app_helpers::api_res_helper::ApiResponse<Json::Value> resp;
    try
    {
        resp = co_await this->fetch->Get("/products/search", queries);
    }
    catch (std::runtime_error &err)
    {
        LOG_ERROR << err.what();
        throw std::runtime_error(err.what());
    }

    co_return resp.data();
}