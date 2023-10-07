#include "demo_v1_User.h"
#include <iostream>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/Exception.h>

#include "models/User.h"
#include "../utils/CryptoHelper.hpp"
#include "../utils/JwtHelper.hpp"
#include "../errors/ResourceNotFoundException.hpp"
#include "../utils/ApiResponse.hpp"
#include "../utils/FtpHelper.hpp"
#include "../utils/FileHelper.hpp"

using namespace std;

using namespace demo::v1;

using UserModel = drogon_model::drogon_test::User;

// Add definition of your processing function here

void User::login(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback)
{

    app_helpers::api_res_helper::ApiResponse::Builder builderRes = app_helpers::api_res_helper::ApiResponse::create();
    std::string data = "", message = "";
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

        data = accessToken;
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

void demo::v1::User::loginView(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    try
    {
        std::string userId = req->getParameter("userId");
        std::string passwd = req->getParameter("password");
        cout << "User: " << userId << endl;

        auto db = drogon::app().getDbClient();

        drogon::orm::Mapper<UserModel> usr(db);

        UserModel user;
        try
        {
            user = usr.findOne(drogon::orm::Criteria(UserModel::Cols::_email, userId));
        }
        catch (orm::UnexpectedRows &ex)
        {
            throw ResourceNotFoundException("User not found!");
        };

        bool match = app_helpers::crypto_helper::matches(passwd, *(user.getPassword()), *(user.getSecurityStamp()));

        cout << "User: " << user.toJson() << *(user.getPassword()) << endl;

        if (match != true)
        {
            throw ResourceNotFoundException("Password not match");
        };

        Json::Value tokenPayload;
        tokenPayload["uid"] = userId;
        tokenPayload["cid"] = "nok.com.vn";

        Json::FastWriter writer;
        string accessToken = app_helpers::jwt_helper::generateAccessToken(writer.write(tokenPayload));

        Cookie cookie("token", accessToken);

        HttpViewData data;
        data["name"] = req->getParameter("name");
        auto resp = HttpResponse::newHttpViewResponse("views::HelloView", data);
        resp->addCookie(cookie);
        callback(resp);
    }
    catch (ResourceNotFoundException &ex)
    {
        auto resp = HttpResponse::newHttpViewResponse("views::user::login");
        callback(resp);
    }
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
    auto resp = HttpResponse::newHttpViewResponse("views::HelloView", data);
    callback(resp);
}

void demo::v1::User::listUserView(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    try
    {
        auto db = drogon::app().getDbClient();

        drogon::orm::Mapper<UserModel> usr(db);

        vector<UserModel> users;
        try
        {
            users = usr.findAll();
        }
        catch (orm::UnexpectedRows &ex)
        {
            throw ResourceNotFoundException("User not found!");
        };
        
        HttpViewData data = HttpViewData();
        data["users"] = users;
        auto resp = HttpResponse::newHttpViewResponse("views::user::user_list", data);
        callback(resp);
    }
    catch (ResourceNotFoundException &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

void demo::v1::User::newUserView(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    try
    {
        HttpViewData data = HttpViewData();
        UserModel user;
        data["user"] = user;
        auto resp = HttpResponse::newHttpViewResponse("views::user::user_form", data);
        callback(resp);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    
}

void demo::v1::User::insertUserView(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto nameUser = req->getParameter("name");
    auto emailUser = req->getParameter("email");


    auto db = drogon::app().getDbClient();
    drogon::orm::Mapper<UserModel> usr(db);

    UserModel user;
    user.setEmail(emailUser);
    user.setName(nameUser);
    usr.insert(user);

    auto resp = HttpResponse::newRedirectionResponse("list");
    callback(resp);
}

void demo::v1::User::editUserView(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string userId)
{
    try
    {
        auto db = drogon::app().getDbClient();

        drogon::orm::Mapper<UserModel> usr(db);

        UserModel user;
        try
        {
            user = usr.findOne(drogon::orm::Criteria(UserModel::Cols::_id, userId));
        }
        catch (orm::UnexpectedRows &ex)
        {
            throw ResourceNotFoundException("User not found!");
        };
        HttpViewData data = HttpViewData();
        data["user"] = user;
        auto resp = HttpResponse::newHttpViewResponse("views::user::user_form", data);
        callback(resp);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    
}

void demo::v1::User::updateUserView(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto idUser = req->getParameter("id");
    auto nameUser = req->getParameter("name");
    auto emailUser = req->getParameter("email");


    auto db = drogon::app().getDbClient();
    drogon::orm::Mapper<UserModel> usr(db);

    UserModel user;

    int idPrimaryKey = stoi(idUser);

    try
    {
        user = usr.findByPrimaryKey(idPrimaryKey);
    }
    catch (orm::UnexpectedRows &ex)
    {
        throw ResourceNotFoundException("User not found!");
    };

    user.setEmail(emailUser);
    user.setName(nameUser);
    usr.update(user);

    auto resp = HttpResponse::newRedirectionResponse("list");
    callback(resp);

}

void demo::v1::User::deleteUserView(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string userId)
{
    try
    {
        auto db = drogon::app().getDbClient();

        drogon::orm::Mapper<UserModel> usr(db);

        UserModel user;
        try
        {
            user = usr.findOne(drogon::orm::Criteria(UserModel::Cols::_id, userId));
        }
        catch (orm::UnexpectedRows &ex)
        {
            throw ResourceNotFoundException("User not found!");
        };

        usr.deleteByPrimaryKey(user.getPrimaryKey());

        auto resp = HttpResponse::newRedirectionResponse("../list");
        callback(resp);
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    
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