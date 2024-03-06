#include "CourseService.hpp"

namespace app_services::course_service
{
    drogon::Task<std::vector<CourseDto>> getAll()
    {
        std::vector<CourseModel> courses = co_await app_repositories::course_repository::getAll();

        std::vector<CourseDto> courseDtos;
        for (auto course : courses)
        {
            CourseDto temp = CourseDto::fromModel(course);
            courseDtos.push_back(temp);
        }
        co_return courseDtos;
    }

    drogon::Task<CourseDto> getById(int id)
    {
        CourseModel course = co_await app_repositories::course_repository::getById(id);
        co_return CourseDto::fromModel(course);
    }

    drogon::Task<CourseDto> create(CourseDto data)
    {
        CourseModel course = CourseDto::toModel(data);
        course.setCreatedDate(::trantor::Date());
        course = co_await app_repositories::course_repository::create(course);
        co_return CourseDto::fromModel(course);
    }

    drogon::Task<bool> update(CourseDto data)
    {
        CourseModel course = CourseDto::toModel(data);
        bool result = co_await app_repositories::course_repository::update(course);
        co_return result;
    }

    drogon::Task<bool> remove(int id)
    {
        bool result = co_await app_repositories::course_repository::remove(id);
        co_return result;
    }

}
