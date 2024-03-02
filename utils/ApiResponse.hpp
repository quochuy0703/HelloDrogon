#pragma once

#include <iostream>
#include <drogon/orm/Mapper.h>

namespace app_helpers::api_res_helper
{
    template <typename T>
    class ApiResponse
    {
    private:
        /* data */
        T data_;
        std::string message_;
        int statusCode_;
        bool success_;

    public:
        class Builder
        {
        private:
            ApiResponse<T> *api;

        public:
            Builder()
            {
                this->api = new ApiResponse<T>();
            };
            Builder &message(const std::string &message)
            {
                this->api->message_ = message;
                return *this;
            };
            Builder &data(const T &data)
            {
                this->api->data_ = data;
                return *this;
            };
            Builder &statusCode(const int &statusCode)
            {
                this->api->statusCode_ = statusCode;
                return *this;
            };
            Builder &success(const bool &success)
            {
                this->api->success_ = success;
                return *this;
            };
            ApiResponse<T> *build()
            {
                return this->api;
            };
        };
        static Builder create()
        {
            return Builder();
        };
        Json::Value toJson()
        {
            Json::Value ret;
            ret["Data"] = this->data_;
            ret["Message"] = this->message_;
            ret["StatusCode"] = this->statusCode_;
            ret["Success"] = this->success_;

            return ret;
        };
    };

}