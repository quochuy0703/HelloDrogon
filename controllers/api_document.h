#pragma once

#include <drogon/HttpController.h>
#include "../dto/UserLoginDto.hpp"
#include "../dto/UserDto.cpp"

#include "../utils/lodash.hpp"
#include "../utils/ArrayHelper.hpp"
#include "../utils/sql.h"
#include "../filters/MyMiddleware.h"

#include "../services/FakeApiService.hpp"

using namespace drogon;
using namespace sql;
using namespace app_services;

namespace api
{

    class document : public drogon::HttpController<document>
    {
    public:
        METHOD_LIST_BEGIN
        // use METHOD_ADD to add your custom processing function here;
        // METHOD_ADD(User::get, "/{2}/{1}", Get); // path is /demo/v1/User/{arg2}/{arg1}
        // METHOD_ADD(User::your_method_name, "/{1}/{2}/list", Get); // path is /demo/v1/User/{arg1}/{arg2}/list
        // ADD_METHOD_TO(User::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list

        METHOD_ADD(document::login, "/auth/login", Post);
        METHOD_ADD(document::getInfo, "/getInfo", Get, "MyMiddleware", "VerifyAccessTokenMiddleware");

        METHOD_LIST_END
        // your declaration of processing function maybe like this:
        // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
        // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
        drogon::AsyncTask login(HttpRequestPtr req,
                                std::function<void(const HttpResponsePtr &)> callback, app_dto::user_login::UserLoginDto &&userLogin);
        drogon::AsyncTask getInfo(HttpRequestPtr req,
                                  std::function<void(const HttpResponsePtr &)> callback) const;
    };

}
