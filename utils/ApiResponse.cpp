#include "ApiResponse.hpp"

Json::Value app_helpers::api_res_helper::ApiResponse::toJson()
{
    return Json::Value();
}

void app_helpers::api_res_helper::ApiResponse::Builder::message(const std::string &message)
{
}

void app_helpers::api_res_helper::ApiResponse::Builder::data(const std::string &data)
{
}

void app_helpers::api_res_helper::ApiResponse::Builder::statusCode(const std::string &statusCode)
{
}

void app_helpers::api_res_helper::ApiResponse::Builder::success(const std::string &success)
{
}
