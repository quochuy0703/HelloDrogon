#pragma once

#include <drogon/HttpController.h>
#include "../services/CourseSqlService.hpp"
#include "../dto/CourseDto.cpp"
#include <trantor/utils/Date.h>
#include "../utils/ApiResponse.hpp"

using namespace drogon;

namespace api
{
  namespace v1
  {
    class Course : public drogon::HttpController<Course>
    {
    public:
      METHOD_LIST_BEGIN
      // use METHOD_ADD to add your custom processing function here;
      // METHOD_ADD(Course::get, "/{2}/{1}", Get); // path is /api/v1/Course/{arg2}/{arg1}
      // METHOD_ADD(Course::your_method_name, "/{1}/{2}/list", Get); // path is /api/v1/Course/{arg1}/{arg2}/list
      // ADD_METHOD_TO(Course::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
      METHOD_ADD(Course::GetAllCourse, "/", Get);
      METHOD_ADD(Course::GetCourseById, "/{1}", Get);
      METHOD_ADD(Course::GetCourseByCondition, "/condition", Get);
      METHOD_ADD(Course::PostCourse, "/", Post);
      METHOD_ADD(Course::UpdateCourse, "/", Put);
      METHOD_ADD(Course::DeleteCourseById, "/{1}", Delete);
      METHOD_LIST_END
      // your declaration of processing function maybe like this:
      // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
      // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
      drogon::AsyncTask GetAllCourse(const HttpRequestPtr req,
                                     std::function<void(const HttpResponsePtr &)> callback);
      drogon::AsyncTask GetCourseByCondition(const HttpRequestPtr req,
                                             std::function<void(const HttpResponsePtr &)> callback);
      drogon::AsyncTask GetCourseById(const HttpRequestPtr req,
                                      std::function<void(const HttpResponsePtr &)> callback, std::string courseId);
      drogon::AsyncTask PostCourse(const HttpRequestPtr req,
                                   std::function<void(const HttpResponsePtr &)> callback, app_dto::course::CourseDto &&course);
      drogon::AsyncTask UpdateCourse(const HttpRequestPtr req,
                                     std::function<void(const HttpResponsePtr &)> callback, app_dto::course::CourseDto &&course);
      drogon::AsyncTask DeleteCourseById(const HttpRequestPtr req,
                                         std::function<void(const HttpResponsePtr &)> callback, std::string courseId);
    };
  }
}
