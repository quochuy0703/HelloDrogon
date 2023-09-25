#include "ApiResponse.hpp"

app_helpers::api_res_helper::ApiResponse::Builder app_helpers::api_res_helper::ApiResponse::create()
{
    return app_helpers::api_res_helper::ApiResponse::Builder();
}

Json::Value app_helpers::api_res_helper::ApiResponse::toJson()
{
    Json::Value ret;
    ret["Data"] = this->data_;
    ret["Message"] = this->message_;
    ret["StatusCode"] = this->statusCode_;
    ret["Success"] = this->success_;

    return ret;
}

app_helpers::api_res_helper::ApiResponse::Builder::Builder()
{
    this->api = new app_helpers::api_res_helper::ApiResponse();
}

app_helpers::api_res_helper::ApiResponse::Builder &app_helpers::api_res_helper::ApiResponse::Builder::message(const std::string &message)
{
    this->api->message_ = message;
    return *this;
}

app_helpers::api_res_helper::ApiResponse::Builder &app_helpers::api_res_helper::ApiResponse::Builder::data(const std::string &data)
{
    this->api->data_ = data;
    return *this;
}

app_helpers::api_res_helper::ApiResponse::Builder &app_helpers::api_res_helper::ApiResponse::Builder::statusCode(const std::string &statusCode)
{
    this->api->statusCode_ = statusCode;
    return *this;
}

app_helpers::api_res_helper::ApiResponse::Builder &app_helpers::api_res_helper::ApiResponse::Builder::success(const std::string &success)
{
    this->api->success_ = success;
    return *this;
}

app_helpers::api_res_helper::ApiResponse *app_helpers::api_res_helper::ApiResponse::Builder::build()
{
    return this->api;
}