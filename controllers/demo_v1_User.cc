#include "demo_v1_User.h"
#include <iostream>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/Exception.h>

#include "models/UserLogin.h"
#include "../utils/CryptoHelper.hpp"
#include "../utils/JwtHelper.hpp"
#include "../errors/ResourceNotFoundException.hpp"

using namespace std;

using namespace demo::v1;

using UserModel = drogon_model::test::UserLogin;

// Add definition of your processing function here

void User::login(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback)
{

    Json::Value ret;

    try
    {
        const auto reqBodyPtr = req->getJsonObject();
        const auto &reqBody = *reqBodyPtr;
        Json::Value userId = reqBody["userId"];
        Json::Value passwd = reqBody["password"];
        cout << "User: " << userId.asString() << endl;

        auto db = drogon::app().getDbClient();

        drogon::orm::Mapper<UserModel> usr(db);
        auto user = usr.findOne(drogon::orm::Criteria(UserModel::Cols::_email, userId.asString()));
        bool match = app_helpers::crypto_helper::matches(passwd.asString(), *(user.getPassword()));
        cout << "User: " << user.toJson() << *(user.getPassword()) << endl;

        if (match != true)
        {
            throw ResourceNotFoundException("Password not match");
        };

        Json::FastWriter writer;

        Json::Value tokenPayload;
        tokenPayload["uid"] = userId.asString();
        tokenPayload["cid"] = "nok.com.vn";

        string accessToken = app_helpers::jwt_helper::generateAccessToken(writer.write(tokenPayload));

        ret["result"] = "ok";
        //   ret["token"] = drogon::utils::getUuid();
        ret["token"] = accessToken;
        //   auto resp = HttpResponse::newHttpJsonResponse(ret);
        //   callback(resp);
    }
    catch (orm::UnexpectedRows &ex)
    {
        cout << "Error: " << ex.what() << endl;
        ret["result"] = "not ok";
        //   ret["token"] = drogon::utils::getUuid();
        ret["token"] = ex.what();
        // auto resp = HttpResponse::newHttpJsonResponse(ret);
        // callback(resp);
    }
    catch (ResourceNotFoundException &ex)
    {
        cout << "Error: " << ex.what() << endl;
    }
    catch (exception &ex)
    {
        cout << "Error: " << ex.what() << endl;
    }

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void User::getInfo(const HttpRequestPtr &req,
                   std::function<void(const HttpResponsePtr &)> &&callback,
                   std::string userId,
                   const std::string &token) const
{

    Json::Value ret;
    ret["result"] = "ok";
    ret["user_name"] = "Jack";
    ret["user_id"] = userId;
    ret["gender"] = 1;
    auto resp = HttpResponse::newHttpJsonResponse(ret);

    callback(resp);
}