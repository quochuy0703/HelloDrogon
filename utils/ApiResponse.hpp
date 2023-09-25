#include <iostream>
#include <drogon/orm/Mapper.h>

namespace app_helpers::api_res_helper
{
    class ApiResponse
    {
    private:
        /* data */
        class Builder
        {
        private:
            app_helpers::api_res_helper::ApiResponse api;
            std::string data_;
            std::string message_;
            std::string statusCode_;
            std::string success_;

        public:
            void message(const std::string &message);
            void data(const std::string &data);
            void statusCode(const std::string &statusCode);
            void success(const std::string &success);
        };

    public:
        Json::Value toJson();
    };

}