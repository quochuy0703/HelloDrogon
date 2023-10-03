#include "demo_v1_User.h"
#include <iostream>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/Exception.h>

#include "models/UserLogin.h"
#include "models/Systemaccounts.h"
#include "../utils/CryptoHelper.hpp"
#include "../utils/JwtHelper.hpp"
#include "../errors/ResourceNotFoundException.hpp"
#include "../utils/ApiResponse.hpp"
#include "../utils/FtpHelper.hpp"
#include "../utils/FileHelper.hpp"

using namespace std;

using namespace demo::v1;

using UserModel = drogon_model::test::UserLogin;
using UserSystemModel = drogon_model::test::Systemaccounts;

// Add definition of your processing function here

void User::login(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback)
{

    app_helpers::api_res_helper::ApiResponse::Builder builderRes = app_helpers::api_res_helper::ApiResponse::create();
    Json::Value data;
    std::string message = "";
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

        UserModel user;
        try
        {
            user = usr.findOne(drogon::orm::Criteria(UserModel::Cols::_email, userId.asString()));
        }
        catch (orm::UnexpectedRows &ex)
        {
            throw ResourceNotFoundException("User not found!");
        };

        bool match = app_helpers::crypto_helper::matches(passwd.asString(), *(user.getPassword()), *(user.getSecurityStamp()));

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

        data["token"] = accessToken;
        message = "OK";
    }
    catch (ResourceNotFoundException &ex)
    {
        cout << "Error: " << ex.what() << endl;
        message = ex.what();
    }
    catch (exception &ex)
    {
        cout << "Error: " << ex.what() << endl;
        message = ex.what();
    }

    ret = builderRes.data(data).message(message).statusCode("200").success("ok").build()->toJson();
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void demo::v1::User::loginAccount(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    app_helpers::api_res_helper::ApiResponse::Builder builderRes = app_helpers::api_res_helper::ApiResponse::create();
    Json::Value data;
    std::string message = "";
    Json::Value ret;

    try
    {
        const auto reqBodyPtr = req->getJsonObject();
        const auto &reqBody = *reqBodyPtr;
        Json::Value userId = reqBody["userId"];
        Json::Value passwd = reqBody["password"];
        cout << "User: " << userId.asString() << endl;

        auto db = drogon::app().getDbClient();

        drogon::orm::Mapper<UserSystemModel> usr(db);

        UserSystemModel user;
        try
        {
            user = usr.findOne(drogon::orm::Criteria(UserSystemModel::Cols::_username, userId.asString()));
        }
        catch (orm::UnexpectedRows &ex)
        {
            throw ResourceNotFoundException("User not found!");
        };

        bool match = app_helpers::crypto_helper::matches(passwd.asString(), *(user.getPasswordhash()), *(user.getSecuritystamp()));

        cout << "User: " << user.toJson() << *(user.getPasswordhash()) << endl;

        if (match != true)
        {
            throw ResourceNotFoundException("Password not match");
        };

        Json::FastWriter writer;

        Json::Value tokenPayload;
        tokenPayload["uid"] = userId.asString();
        tokenPayload["eid"] = *(user.getEmail());
        tokenPayload["cid"] = "nok.com.vn";
        tokenPayload["sid"] = *(user.getSession());

        string accessToken = app_helpers::jwt_helper::generateAccessToken(writer.write(tokenPayload));

        Json::Value accountJson;
        accountJson["id"] = *user.getAccountid();
        accountJson["username"] = *user.getUsername();
        accountJson["display_name"] = *user.getDisplayname();
        accountJson["email"] = *user.getEmail();
        accountJson["avatar"] = *user.getAvatar();
        accountJson["isFirstTimeLogin"] = *user.getIsfirsttimelogin();
        accountJson["isHRReference"] = *user.getIshrreference();
        accountJson["status"] = *user.getStatus();
        accountJson["type"] = *user.getType();
        Json::Value tokenJson;
        tokenJson["access_token"] = accessToken;
        tokenJson["refresh_token"] = accessToken;

        Json::Value resJson;
        resJson["Account"] = accountJson;
        resJson["Token"] = tokenJson;

        data = resJson;

        message = "OK";
    }
    catch (ResourceNotFoundException &ex)
    {
        cout << "Error: " << ex.what() << endl;
        message = ex.what();
    }
    catch (exception &ex)
    {
        cout << "Error: " << ex.what() << endl;
        message = ex.what();
    }

    ret = builderRes.data(data).message(message).statusCode("200").success("ok").build()->toJson();
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void demo::v1::User::upload(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    MultiPartParser mpp;
    std::string ftpDir = drogon::app().getCustomConfig()["FTP_UPLOAD_DIR"].asString();
    try
    {
        if (mpp.parse(req) != 0)
        {
            throw std::runtime_error("Error");
        }
    }
    catch (std::exception &ex)
    {
        std::cout << "Error upload file: " << ex.what() << std::endl;
    }

    auto &params = mpp.getParameters();
    auto Files = mpp.getFiles();
    Json::Value ret;
    ret["status"] = "ok";
    std::string message = "";
    for (auto i : params)
        message = message + i.first + " " + i.second;
    std::string fileUpload;
    for (auto i : Files)
    {
        message.append(i.getFileName());
        fileUpload = drogon::utils::getUuid().append(".").append(i.getFileExtension());
        i.saveAs(fileUpload);
        app_helpers::ftp_helper::FtpHelper ftpClient{};
        ftpClient.connect();
        ftpClient.uploadFile("./uploads/" + fileUpload, ftpDir + fileUpload);
        ftpClient.close();
        app_helpers::file_helper::removeFile("uploads/" + fileUpload);
    }
    ret["message"] = message;
    auto resp(HttpResponse::newHttpJsonResponse(ret));
    callback(resp);
}

void demo::v1::User::helloView(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    HttpViewData data;
    data["name"] = req->getParameter("name");
    auto resp = HttpResponse::newHttpViewResponse("HelloView", data);
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