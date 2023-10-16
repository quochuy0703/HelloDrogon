#include <iostream>
#include <drogon/orm/Mapper.h>

namespace app_helpers::api_res_helper
{
    class ApiResponse
    {
    private:
        /* data */
        Json::Value data_;
        std::string message_;
        std::string statusCode_;
        std::string success_;

    public:
        class Builder
        {
        private:
            app_helpers::api_res_helper::ApiResponse *api;

        public:
            Builder();
            Builder &message(const std::string &message);
            Builder &data(const Json::Value &data);
            Builder &statusCode(const std::string &statusCode);
            Builder &success(const std::string &success);
            app_helpers::api_res_helper::ApiResponse *build();
        };

        static Builder create();
        Json::Value toJson();
    };

}