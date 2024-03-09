#include "CourseSqlService.hpp"

namespace app_services::course_service
{
    drogon::Task<std::vector<CourseDto>> getAllSql()
    {

        std::vector<CourseModel> courses;
        try
        {
            auto db = drogon::app().getDbClient();
            auto tranPtr = co_await db->newTransactionCoro();

            courses = co_await app_repositories::course_repository::getAllSql(tranPtr.get());
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

    drogon::Task<CourseDto> getByIdSql(int id)
    {
        CourseModel course;
        auto db = drogon::app().getDbClient();
        auto tranPtr = co_await db->newTransactionCoro();
        try
        {
            course = co_await app_repositories::course_repository::getByIdSql(tranPtr, id);
        }
        catch (const std::exception &ex)
        {
            tranPtr->rollback();
            throw std::runtime_error(ex.what());
        }
        co_return CourseDto::fromModel(course);
    }

    drogon::Task<CourseDto> createSql(CourseDto data)
    {
        CourseModel course = CourseDto::toModel(data);
        course.setCreatedDate(::trantor::Date().now());

        auto db = drogon::app().getDbClient();
        auto tranPtr = co_await db->newTransactionCoro();

        try
        {
            course = co_await app_repositories::course_repository::createSql(tranPtr.get(), course);
        }
        catch (const std::exception &ex)
        {
            tranPtr->rollback();
            throw std::runtime_error(ex.what());
        }

        co_return CourseDto::fromModel(course);
    }

    drogon::Task<bool> updateSql(CourseDto data)
    {
        auto db = drogon::app().getDbClient();
        auto tranPtr = co_await db->newTransactionCoro();

        bool result;
        try
        {
            CourseModel course = CourseDto::toModel(data);
            course.setLastModifiedDate(::trantor::Date().now());
            result = co_await app_repositories::course_repository::updateSql(tranPtr, course);
        }
        catch (const std::exception &ex)
        {
            tranPtr->rollback();
            throw std::runtime_error(ex.what());
        }

        co_return result;
    }

    drogon::Task<bool> removeSql(int id)
    {

        bool result;
        auto db = drogon::app().getDbClient();
        auto tranPtr = co_await db->newTransactionCoro();
        try
        {
            result = co_await app_repositories::course_repository::removeSql(tranPtr, id);
        }
        catch (const std::exception &ex)
        {
            tranPtr->rollback();
            throw std::runtime_error(ex.what());
        }
        co_return result;
    }

}
