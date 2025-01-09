#include "api_document.h"
#include <iostream>
#include <sstream>
#include <drogon/orm/Mapper.h>
#include <drogon/orm/Exception.h>

#include "models/UserLogin.h"
#include "models/Systemaccounts.h"
#include "models/Instructor.h"
#include "models/Instructordetail.h"
#include "models/Course.h"
#include "models/Review.h"

#include "../utils/CryptoHelper.hpp"
#include "../utils/JwtHelper.hpp"
#include "../errors/ResourceNotFoundException.hpp"
#include "../utils/ApiResponse.hpp"
#include "../utils/FtpHelper.hpp"
#include "../utils/FileHelper.hpp"
#include "../utils/Utils.hpp"
#include "../utils/Fetch.hpp"
#include "../constants/app.hpp"

#include "../services/UserService.hpp"

#include "../plugins/SMTPMail.h"

using namespace std;

using namespace api;

using UserModel = drogon_model::test::UserLogin;
using UserSystemModel = drogon_model::test::Systemaccounts;
using InstructorModel = drogon_model::test::Instructor;
using InstructorDetailModel = drogon_model::test::Instructordetail;
using CourseModel = drogon_model::test::Course;
using ReviewModel = drogon_model::test::Review;

// Hàm xử lý cho từng bước
std::string start(const HttpRequestPtr req);

std::string createRequest(const HttpRequestPtr req);

std::string sectionApproval(const HttpRequestPtr req);

std::string qcConfirm(const HttpRequestPtr req);

std::string sectionFeedback(const HttpRequestPtr req);

std::string qcApproval(const HttpRequestPtr req);

std::string sectionAddFile(const HttpRequestPtr req);

std::string departmentQCApproval(const HttpRequestPtr req);

// Bản đồ ánh xạ từ các bước SAR_STEPS đến hàm xử lý tương ứng
std::unordered_map<constants::SAR_STEPS, std::function<std::string(const HttpRequestPtr)>> STEP_HANDLERS = {
    {constants::SAR_STEPS::START, start},
    {constants::SAR_STEPS::CREATE_REQUEST, createRequest},
    {constants::SAR_STEPS::SECTION_APPROVAL, sectionApproval},
    {constants::SAR_STEPS::QC_CONFIRM, qcConfirm},
    {constants::SAR_STEPS::SECTION_FEEDBACK, sectionFeedback},
    {constants::SAR_STEPS::QC_APPROVAL, qcApproval},
    {constants::SAR_STEPS::SECTION_ADD_FILE, sectionAddFile},
    {constants::SAR_STEPS::DEPARTMENT_QC_APPROVAL, departmentQCApproval}};

// Add definition of your processing function here

drogon::AsyncTask document::login(HttpRequestPtr req,
                                  std::function<void(const HttpResponsePtr &)> callback, app_dto::user_login::UserLoginDto &&userLogin)
{

    app_helpers::api_res_helper::ApiResponse<Json::Value>::Builder builderRes = app_helpers::api_res_helper::ApiResponse<Json::Value>::create();
    Json::Value data;
    std::string message = "";
    Json::Value ret;

    try
    {
        Json::Value userId = userLogin.userId;
        Json::Value passwd = userLogin.password;

        LOG_INFO << "User: " << userId.asString();

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

        // bool match = app_helpers::crypto_helper::matches(passwd.asString(), *(user.getPassword()), *(user.getSecurityStamp()));
        bool match = co_await app_helpers::crypto_helper::matchesCoro(passwd.asString(), *(user.getPassword()), *(user.getSecurityStamp()));
        cout << "User: " << user.toJson() << *(user.getPassword()) << endl;

        if (match != true)
        {
            throw ResourceNotFoundException("Password not match");
        };

        Json::FastWriter writer;

        Json::Value tokenPayload;
        tokenPayload["uid"] = userId.asString();
        tokenPayload["cid"] = "nok.com.vn";

        // string accessToken = app_helpers::jwt_helper::generateAccessToken(writer.write(tokenPayload));
        string accessToken = co_await app_helpers::jwt_helper::generateAccessTokenCoro(writer.write(tokenPayload));

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

    ret = builderRes.data(data).message(message).statusCode(drogon::HttpStatusCode::k200OK).success("ok").build()->toJson();
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

// if use return drogon::AsyncTask  --> must use co_await ...,
drogon::AsyncTask document::getInfo(HttpRequestPtr req,
                                    std::function<void(const HttpResponsePtr &)> callback) const
{
    Json::Value ret;

    try
    {
        Json::FastWriter writer;

        FakeApiService fetch;
        auto data = co_await fetch.searchProduct({{"q", "phone"}});
        ret["resp"] = data;

        Json::Value tokenPayload;
        tokenPayload["cid"] = "nok.com.vn";

        auto uid = req->getParameter("username");

        string accessToken = co_await app_helpers::jwt_helper::generateAccessTokenCoro(writer.write(tokenPayload));

        ret["result"] = "ok";
        // ret["token"] = accessToken;
    }
    catch (exception &ex)
    {
        cout << "Error: " << ex.what() << endl;
        ret["error"] = ex.what();
    }
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

drogon::AsyncTask document::submit(const HttpRequestPtr req,
                                   std::function<void(const HttpResponsePtr &)> callback)
{
    Json::Value ret;

    try
    {
        // Chạy hàm tương ứng step hiện tại
        constants::SAR_STEPS currentStep = constants::SAR_STEPS::CREATE_REQUEST;
        auto response = STEP_HANDLERS[currentStep](req);

        Json::FastWriter writer;

        Json::Value tokenPayload;
        tokenPayload["cid"] = "nok.com.vn";

        auto uid = req->getParameter("username");

        auto *DIPluginPtr = app().getPlugin<drogon::plugin::DIFruitPlugin>();

        const auto uuidRequest = req->getParameter("uuidRequest");
        auto pcPartService = DIPluginPtr->get<ISystemSerialDetailService>(uuidRequest);

        // LOG_INFO << "userService ptr count: " << pcPartService.use_count();

        auto listPart = co_await pcPartService->GetAllDto();

        string accessToken = co_await app_helpers::jwt_helper::generateAccessTokenCoro(writer.write(tokenPayload));

        ret["result"] = "ok";
        ret["step"] = response;
        ret["uid"] = uid;
        // ret["token"] = accessToken;
    }
    catch (exception &ex)
    {
        cout << "Error: " << ex.what() << endl;
        ret["error"] = ex.what();
    }
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

std::string start(const HttpRequestPtr req)
{
    std::cout << "Executing start step." << std::endl;
    return "start";
}

std::string createRequest(const HttpRequestPtr req)
{
    std::cout << "Executing createRequest step." << std::endl;
    return "createRequest";
}

std::string sectionApproval(const HttpRequestPtr req)
{
    std::cout << "Executing sectionApproval step." << std::endl;
    return "sectionApproval";
}

std::string qcConfirm(const HttpRequestPtr req)
{
    std::cout << "Executing qcConfirm step." << std::endl;
    return "qcConfirm";
}

std::string sectionFeedback(const HttpRequestPtr req)
{
    std::cout << "Executing sectionFeedback step." << std::endl;
    return "sectionFeedback";
}

std::string qcApproval(const HttpRequestPtr req)
{
    std::cout << "Executing qcApproval step." << std::endl;
    return "qcApproval";
}

std::string sectionAddFile(const HttpRequestPtr req)
{
    std::cout << "Executing sectionAddFile step." << std::endl;
    return "sectionAddFile";
}

std::string departmentQCApproval(const HttpRequestPtr req)
{
    std::cout << "Executing departmentQCApproval step." << std::endl;
    return "departmentQCApproval";
}
