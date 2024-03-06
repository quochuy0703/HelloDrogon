#include "api_v1_Course.h"

using namespace api::v1;

// Add definition of your processing function here

drogon::AsyncTask api::v1::Course::GetAllCourse(const HttpRequestPtr req,
                                                std::function<void(const HttpResponsePtr &)> callback)
{
    Json::Value ret;
    ret["result"] = "ok";

    auto cousers = co_await app_services::course_service::getAll();

    for (std::vector<app_dto::course::CourseDto>::iterator it = cousers.begin(); it != cousers.end(); it++)
    {
        Json::Value jsonVect;
        // jsonVect.append(*it);
        jsonVect["name"] = it->name;
        jsonVect["code"] = it->code;
        ret["courses"].append(jsonVect);
    };

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

drogon::AsyncTask api::v1::Course::PostCourse(const HttpRequestPtr req,
                                              std::function<void(const HttpResponsePtr &)> callback, app_dto::course::CourseDto &&course)
{
    auto courseDto = co_await app_services::course_service::create(course);
    Json::Value ret;
    ret["result"] = "ok";
    Json::Value jsonVect;
    jsonVect["name"] = courseDto.name;
    jsonVect["code"] = courseDto.code;
    jsonVect["id"] = courseDto.id;
    ret["course"].append(jsonVect);
    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}

drogon::AsyncTask api::v1::Course::UpdateCourse(const HttpRequestPtr req,
                                                std::function<void(const HttpResponsePtr &)> callback, app_dto::course::CourseDto &&course)
{
    auto result = co_await app_services::course_service::update(course);
    Json::Value ret;
    ret["result"] = ret;

    auto resp = HttpResponse::newHttpJsonResponse(ret);
    callback(resp);
}
