#include <iostream>
#include <jsoncpp/json/json.h>


namespace app_helpers::api_res_helper{
    class ApiResponse
    {
        private:
            /* data */
            std::string data_;
            std::string message_;
            std::string statusCode_;
            std::string success_;
        public:
            void message(const std::string &message);
            void data(const std::string &data);
            void statusCode(const std::string &statusCode);
            void success(const std::string &success);
            Json::Value toJson();
        };
         
}