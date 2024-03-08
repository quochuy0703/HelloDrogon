#include "api_v1_Course.h"

using namespace api::v1;

// Add definition of your processing function here

drogon::AsyncTask api::v1::Course::GetAllCourse(const HttpRequestPtr req,
                                                std::function<void(const HttpResponsePtr &)> callback)
{
    Json::Value data;
    app_helpers::api_res_helper::ApiResponse<Json::Value>::Builder builderRes = app_helpers::api_res_helper::ApiResponse<Json::Value>::create();
    std::string message = "";
    drogon::HttpStatusCode statusCode;
    try
    {
        auto cousers = co_await app_services::course_service::getAll();
        for (std::vector<app_dto::course::CourseDto>::iterator it = cousers.begin(); it != cousers.end(); it++)
        {
            Json::Value jsonVect;
            // jsonVect.append(*it);
            jsonVect["name"] = it->name;
            jsonVect["code"] = it->code;
            data["courses"].append(jsonVect);
        };

        message = "success";
        statusCode = drogon::HttpStatusCode::k200OK;
    }
    catch (const std::exception &ex)
    {
        message = ex.what();
        statusCode = drogon::HttpStatusCode::k500InternalServerError;
    }

    Json::Value ret = builderRes.data(data).message(message).success(true).statusCode(statusCode).build()->toJson();
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

drogon::AsyncTask api::v1::Course::GetCourseById(const HttpRequestPtr req,
                                                 std::function<void(const HttpResponsePtr &)> callback, std::string courseId)
{
    Json::Value data;
    app_helpers::api_res_helper::ApiResponse<Json::Value>::Builder builderRes = app_helpers::api_res_helper::ApiResponse<Json::Value>::create();
    std::string message = "";
    drogon::HttpStatusCode statusCode;

    try
    {
        auto course = co_await app_services::course_service::getById(std::stoi(courseId));

        if (course.id.has_value())
        {
            data["id"] = course.id.value();
        }

        data["name"] = course.name;
        data["code"] = course.code;

        message = "success";
        statusCode = drogon::HttpStatusCode::k200OK;
    }
    catch (const std::exception &ex)
    {
        message = ex.what();
        statusCode = drogon::HttpStatusCode::k404NotFound;
    }

    Json::Value ret = builderRes.data(data).message(message).success(true).statusCode(statusCode).build()->toJson();
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

drogon::AsyncTask api::v1::Course::PostCourse(const HttpRequestPtr req,
                                              std::function<void(const HttpResponsePtr &)> callback, app_dto::course::CourseDto &&course)
{
    Json::Value ret;
    try
    {

        auto courseDto = co_await app_services::course_service::create(course);

        ret["result"] = "ok";
        Json::Value jsonVect;
        jsonVect["name"] = courseDto.name;
        jsonVect["code"] = courseDto.code;

        if (courseDto.id.has_value())
        {
            jsonVect["id"] = courseDto.id.value();
        }

        ret["course"].append(jsonVect);
    }
    catch (const std::exception &ex)
    {
        ret["result"] = ex.what();
    }
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

drogon::AsyncTask api::v1::Course::UpdateCourse(const HttpRequestPtr req,
                                                std::function<void(const HttpResponsePtr &)> callback, app_dto::course::CourseDto &&course)
{
    Json::Value data;
    app_helpers::api_res_helper::ApiResponse<Json::Value>::Builder builderRes = app_helpers::api_res_helper::ApiResponse<Json::Value>::create();
    std::string message = "";
    drogon::HttpStatusCode statusCode;
    try
    {
        auto result = co_await app_services::course_service::update(course);

        data["result"] = result;
        message = "success";
        statusCode = drogon::HttpStatusCode::k200OK;
    }
    catch (const std::exception &ex)
    {
        data["result"] = false;
        message = ex.what();
        statusCode = drogon::HttpStatusCode::k404NotFound;
    }
    Json::Value ret = builderRes.data(data).message(message).success(true).statusCode(statusCode).build()->toJson();
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

drogon::AsyncTask api::v1::Course::DeleteCourseById(const HttpRequestPtr req,
                                                    std::function<void(const HttpResponsePtr &)> callback, std::string courseId)
{
    Json::Value data;
    app_helpers::api_res_helper::ApiResponse<Json::Value>::Builder builderRes = app_helpers::api_res_helper::ApiResponse<Json::Value>::create();
    std::string message = "";
    drogon::HttpStatusCode statusCode;
    try
    {
        auto result = co_await app_services::course_service::remove(std::stoi(courseId));

        data["result"] = result;
        message = "success";
        statusCode = drogon::HttpStatusCode::k200OK;
    }
    catch (const std::exception &ex)
    {
        data["result"] = false;
        message = ex.what();
        statusCode = drogon::HttpStatusCode::k404NotFound;
    }
    Json::Value ret = builderRes.data(data).message(message).success(true).statusCode(statusCode).build()->toJson();
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
