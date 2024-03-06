#include "CourseService.hpp"

namespace app_services::course_service
{
    drogon::Task<std::vector<CourseDto>> getAll()
    {

        std::vector<CourseModel> courses;
        try
        {
            courses = co_await app_repositories::course_repository::getAll();
        }
        catch (const std::exception &ex)
        {
            throw std::runtime_error(ex.what());
        }
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
        CourseModel course;
        try
        {
            course = co_await app_repositories::course_repository::getById(id);
        }
        catch (const std::exception &ex)
        {
            throw std::runtime_error(ex.what());
        }
        co_return CourseDto::fromModel(course);
    }

    drogon::Task<CourseDto> create(CourseDto data)
    {
        CourseModel course = CourseDto::toModel(data);
        course.setCreatedDate(::trantor::Date().now());
        try
        {
            course = co_await app_repositories::course_repository::create(course);
        }
        catch (const std::exception &ex)
        {
            throw std::runtime_error(ex.what());
        }

        co_return CourseDto::fromModel(course);
    }

    drogon::Task<bool> update(CourseDto data)
    {
        bool result;
        try
        {
            CourseModel course = CourseDto::toModel(data);
            course.setLastModifiedDate(::trantor::Date().now());
            result = co_await app_repositories::course_repository::update(course);
        }
        catch (const std::exception &ex)
        {
            throw std::runtime_error(ex.what());
        }

        co_return result;
    }

    drogon::Task<bool> remove(int id)
    {
        bool result = co_await app_repositories::course_repository::remove(id);
        co_return result;
    }

}
