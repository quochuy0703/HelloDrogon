#include "demo_v1_User.h"
#include <iostream>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/Exception.h>

#include "models/User.h"
#include "models/Systemaccounts.h"
#include "../utils/CryptoHelper.hpp"
#include "../utils/JwtHelper.hpp"
#include "../errors/ResourceNotFoundException.hpp"
#include "../utils/ApiResponse.hpp"
#include "../utils/FtpHelper.hpp"
#include "../utils/FileHelper.hpp"
#include "../utils/Utils.hpp"
#include "../utils/Fetch.hpp"

#include "../services/UserService.hpp"

#include "../plugins/SMTPMail.h"

using namespace std;

using namespace demo::v1;

using UserModel = drogon_model::drogon_test::User;
using UserSystemModel = drogon_model::drogon_test::Systemaccounts;

// Add definition of your processing function here

void User::login(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback, app_dto::user_login::UserLoginDto &&userLogin)
{

    app_helpers::api_res_helper::ApiResponse<std::string>::Builder builderRes = app_helpers::api_res_helper::ApiResponse<std::string>::create();
    std::string data = "", message = "";

    Json::Value ret;

    try
    {

        Json::Value userId = userLogin.userId;
        Json::Value passwd = userLogin.password;

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

    ret = builderRes.data(data).message(message).statusCode(drogon::HttpStatusCode::k200OK).success("ok").build()->toJson();
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

void demo::v1::User::loginView(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    try
    {
        if (req->getMethod() == drogon::HttpMethod::Get)
        {
            auto res = drogon::HttpResponse::newHttpViewResponse("views::user::login");
            callback(res);
            return;
        }
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
        // auto resp = HttpResponse::newHttpViewResponse("views::HelloView", data);
        auto resp = HttpResponse::newRedirectionResponse("view");
        resp->addCookie(cookie);
        callback(resp);
    }
    catch (ResourceNotFoundException &ex)
    {
        auto resp = HttpResponse::newHttpViewResponse("views::user::login");
        callback(resp);
    }
}

drogon::AsyncTask demo::v1::User::upload(const HttpRequestPtr req, std::function<void(const HttpResponsePtr &)> callback)
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
        cout << "truoc save file" << endl;
        message.append(i.getFileName());
        fileUpload = drogon::utils::getUuid().append(".").append(i.getFileExtension());
        i.saveAs(fileUpload);
        cout << "sau save file,truoc upload" << endl;
        app_helpers::ftp_helper::FtpHelper ftpClient{};
        ftpClient.connect();
        std::cout << "Main Thread: " << std::this_thread::get_id() << std::endl;
        // auto func = [&]() -> drogon::Task<>
        // {
        //     std::cout << "Sub Thread: " << std::this_thread::get_id() << std::endl;
        //     auto square = co_await ftpClient.uploadFile("./uploads/" + fileUpload, ftpDir + fileUpload);
        // };
        // co_await func();

        // drogon::app().getLoop()->queueInLoop(drogon::async_func([&] -> drogon::Task<>
        //                                                         { co_await ftpClient.uploadFile("./uploads/" + fileUpload, ftpDir + fileUpload); }));
        // // co_await ftpClient.uploadFile("./uploads/" + fileUpload, ftpDir + fileUpload);

        // co_await executeIntensiveFunction([&]()
        //                                   {
        //                                     std::cout << "Sub Thread: " << std::this_thread::get_id() << std::endl;
        //         ftpClient.uploadFile0("./uploads/" + fileUpload, ftpDir + fileUpload);
        //         return; });

        co_await ftpClient.uploadFileCoro("./uploads/" + fileUpload, ftpDir + fileUpload);

        cout << "sau upload file" << endl;
        ftpClient.close();
        app_helpers::file_helper::removeFile("uploads/" + fileUpload);
    }
    ret["message"] = message;
    auto resp(HttpResponse::newHttpJsonResponse(ret));
    callback(resp);
}

void demo::v1::User::loginAccount(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    app_helpers::api_res_helper::ApiResponse<Json::Value>::Builder builderRes = app_helpers::api_res_helper::ApiResponse<Json::Value>::create();
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
            user = usr.findOne(drogon::orm::Criteria(UserSystemModel::Cols::_Username, userId.asString()));
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
        // accountJson["avatar"] = *user.getAvatar();
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

    ret = builderRes.data(data).message(message).statusCode(drogon::HttpStatusCode::k200OK).success("ok").build()->toJson();
    auto resp = HttpResponse::newHttpJsonResponse(ret);
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
        usr.findAll([=](auto users)
                    {
                        HttpViewData data = HttpViewData();
                        data["users"] = users;
                        auto resp = HttpResponse::newHttpViewResponse("views::user::user_list", data);
                        callback(resp); },
                    [=](auto &ex)
                    {
                        LOG_ERROR << ex.base().what();
                        auto resp = HttpResponse::newHttpViewResponse("views::user::user_form");
                        callback(resp);
                    });
        // try
        // {
        //     users = usr.findAll();
        // }
        // catch (orm::UnexpectedRows &ex)
        // {
        //     throw ResourceNotFoundException("User not found!");
        // };

        // HttpViewData data = HttpViewData();
        // data["users"] = users;
        // auto resp = HttpResponse::newHttpViewResponse("views::user::user_list", data);
        // callback(resp);
    }
    catch (ResourceNotFoundException &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

drogon::AsyncTask demo::v1::User::listUserViewCoro(const HttpRequestPtr req,
                                                   std::function<void(const HttpResponsePtr &)> callback)
{
    auto users = co_await app_services::user::getAll();

    HttpViewData data = HttpViewData();
    data["users"] = users;
    auto resp = HttpResponse::newHttpViewResponse("views::user::user_list_coro", data);
    callback(resp);
};

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

void demo::v1::User::newUserViewCoro(const HttpRequestPtr &req,
                                     std::function<void(const HttpResponsePtr &)> &&callback)
{
    try
    {
        HttpViewData data = HttpViewData();
        app_dto::user::UserDto user;
        data["user"] = user;
        auto resp = HttpResponse::newHttpViewResponse("views::user::user_form_coro", data);
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
    usr.insert(
        user, [=](auto result)
        {
            auto resp = HttpResponse::newRedirectionResponse("list");
            callback(resp); },
        [=](auto &ex)
        {
            LOG_ERROR << ex.base().what();
            auto resp = HttpResponse::newHttpViewResponse("views::user::user_form");
            callback(resp);
        });
}

drogon::AsyncTask demo::v1::User::insertUserViewCoro(const HttpRequestPtr req,
                                                     std::function<void(const HttpResponsePtr &)> callback, app_dto::user::UserDto &&user)
{
    auto users = co_await app_services::user::create(user);
    auto resp = HttpResponse::newRedirectionResponse("listCoro");
    callback(resp);
}

void demo::v1::User::editUserView(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback, std::string userId)
{
    try
    {
        auto db = drogon::app().getDbClient();

        drogon::orm::Mapper<UserModel> usr(db);

        // user = usr.findOne(drogon::orm::Criteria(UserModel::Cols::_id, userId));
        // auto callbackPtr = make_shared<function<void(const HttpResponsePtr &)>>(move(callback));
        usr.findOne(
            drogon::orm::Criteria(UserModel::Cols::_id, userId), [=](auto userReturn)
            { 
                    LOG_INFO << *userReturn.getEmail(); 
                    HttpViewData data = HttpViewData();
                    data["user"] = userReturn;
                    auto resp = HttpResponse::newHttpViewResponse("views::user::user_form", data);
                    callback(resp); },
            [callback](auto &ex)
            {
                LOG_ERROR << ex.base().what();
                auto resp = HttpResponse::newHttpViewResponse("views::user::user_form");
                callback(resp);
            });
    }
    catch (std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
}

drogon::AsyncTask demo::v1::User::editUserViewCoro(const HttpRequestPtr req,
                                                   std::function<void(const HttpResponsePtr &)> callback, std::string userId)
{
    auto user = co_await app_services::user::getById(std::stoi(userId));
    LOG_INFO << user.email << ":" << user.id;
    HttpViewData data = HttpViewData();
    data["user"] = user;
    auto resp = HttpResponse::newHttpViewResponse("views::user::user_form_coro", data);
    callback(resp);
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

drogon::AsyncTask demo::v1::User::updateUserViewCoro(const HttpRequestPtr req,
                                                     std::function<void(const HttpResponsePtr &)> callback, app_dto::user::UserDto &&user)
{
    auto result = co_await app_services::user::update(user);
    LOG_INFO << result;
    auto resp = HttpResponse::newRedirectionResponse("listCoro");
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

drogon::AsyncTask demo::v1::User::deleteUserViewCoro(const HttpRequestPtr req,
                                                     std::function<void(const HttpResponsePtr &)> callback, std::string userId)
{
    auto users = co_await app_services::user::remove(std::stoi(userId));
    auto resp = HttpResponse::newRedirectionResponse("../listCoro");
    callback(resp);
}

void demo::v1::User::csfrUserView(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{

    HttpViewData data = HttpViewData();
    data["user"] = "Huy";
    auto resp = HttpResponse::newHttpViewResponse("views::user::user_csfr", data);
    callback(resp);
}

void User::postMail(const HttpRequestPtr &req, std::function<void(const HttpResponsePtr &)> &&callback)
{
    auto *smtpmailPtr = app().getPlugin<SMTPMail>();
    auto id = smtpmailPtr->sendEmail(
        "127.0.0.1",                 // The server IP/DNS
        1025,                        // The port
        "mailer@something.com",      // Who send the email
        "receiver@otherthing.com",   // Send to whom
        "Testing SMTPMail Function", // Email Subject/Title
        "Hello from drogon plugin",  // Content
        "mailer@something.com",      // Login user
        "123456",                    // User password
        false,                       // Is HTML content
        [](const string &msg)
        {
            std::cout << msg << std::endl;
        });

    Json::Value ret;
    ret["result"] = "ok";
    ret["user_name"] = "Jack";
    ret["gender"] = 1;
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

drogon::AsyncTask User::getInfo(const HttpRequestPtr req,
                                std::function<void(const HttpResponsePtr &)> callback,
                                std::string userId,
                                const std::string &token) const
{
    auto att = req->getAttributes();
    auto data = att->get<string>("decoded");

    // auto client = drogon::HttpClient::newHttpClient("http://localhost:8000");
    // auto httpReq = drogon::HttpRequest::newHttpRequest();
    // // httpReq->setPath("/products/1");
    // httpReq->setPath("/");
    // auto result = co_await client->sendRequestCoro(httpReq);

    // auto client = drogon::HttpClient::newHttpClient("https://dummyjson.com");
    // auto httpReq = drogon::HttpRequest::newHttpRequest();
    // httpReq->setMethod(drogon::HttpMethod::Post);
    // httpReq->setPath("/products/add");
    // httpReq->setContentTypeCode(drogon::CT_APPLICATION_JSON);
    // Json::Value reqBody;
    // reqBody["title"] = "BMW Pencil";
    // httpReq->setBody(reqBody.toStyledString());
    // auto result = co_await client->sendRequestCoro(httpReq);

    // Json::Value reqBody;
    // reqBody["title"] = "BMW Pencil";
    // app_helpers::fetch_helper::Fetch fetch;
    // ApiResponse<Json::Value> result = co_await fetch.Post("/products/add", reqBody, "https://dummyjson.com");

    // https://dummyjson.com/products/search?q=phone'
    // app_helpers::fetch_helper::Fetch fetch;
    // map<std::string, std::string> query = {{"q", "phone"}};
    // ApiResponse<Json::Value> result = co_await fetch.Get("/products/search", query, "https://dummyjson.com");

    // https://dummyjson.com/products/1
    // Json::Value reqBody;
    // reqBody["title"] = "iPhone Galaxy +1";
    // app_helpers::fetch_helper::Fetch fetch;
    // ApiResponse<Json::Value> result = co_await fetch.Put("/products/1", reqBody, "https://dummyjson.com");

    // https://dummyjson.com/products/1
    app_helpers::fetch_helper::Fetch fetch;
    ApiResponse<Json::Value> result = co_await fetch.Delete("/products/1", "https://fdfdfdummyjson.com");

    app_helpers::array_helper::Array<int> numbers;

    numbers.concat({1, 2, 10, 5, 7});

    Json::Value ret;
    ret["result"] = "ok";
    ret["user_name"] = "Jack";
    ret["user_id"] = userId;
    ret["data"] = data;
    ret["array"] = numbers.filter([](auto item, auto index)
                                  { return item > 5; })
                       .reduce([](auto acc, auto item, auto index)
                               { return acc + item; },
                               0);
    ret["moment"] = moment::moment().toString();
    // ret["coro"] = square;
    // ret["post"] = *(result->getJsonObject());
    // ret["post"] = result.toJson();
    // ret["get"] = result.toJson();
    // ret["put"] = result.toJson();
    ret["delete"] = result.toJson();
    ret["hmac"] = co_await app_helpers::crypto_helper::generateHMACCoro("HMAC test");
    ret["gender"] = 1;
    ret["format"] = app_helpers::format("{0} - {1} - {2}", 1, "hello", 2.12);
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

drogon::AsyncTask demo::v1::User::getUserByCondition(HttpRequestPtr req, std::function<void(const HttpResponsePtr &)> callback, std::string userId) const
{
    Json::Value ret;
    ret["result"] = "ok";

    std::map<std::string, std::string> condition;
    condition["name"] = req->getParameter("name");
    condition["email"] = req->getParameter("email");
    condition["id"] = req->getParameter("id");

    auto users = co_await app_services::user::getByCondition(condition);
    // auto users = co_await app_services::user::getAllBySql();

    for (std::vector<app_dto::user::UserDto>::iterator it = users.begin(); it != users.end(); it++)
    {
        Json::Value jsonVect;
        // jsonVect.append(*it);
        jsonVect["name"] = it->name;
        jsonVect["email"] = it->email;
        ret["user"].append(jsonVect);
    }

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
    ;
}