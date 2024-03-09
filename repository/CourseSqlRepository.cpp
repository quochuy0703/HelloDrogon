#include "CourseRepository.hpp"

namespace app_repositories::course_repository
{
    drogon::Task<std::vector<Model>> getAllSql(drogon::orm::Transaction *tranPtr)
    {
        std::vector<Model> courses;
        try
        {
            auto rows = co_await tranPtr->execSqlCoro("SELECT * FROM COURSE;");
            for (auto row : rows)
            {
                courses.push_back(Model(row));
            }
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return courses;
    }
    drogon::Task<Model> getByIdSql(drogon::orm::Transaction *tranPtr, int id)
    {
        Model course;

        try
        {

            auto row = co_await tranPtr->execSqlCoro("SELECT * FROM COURSE WHERE id = $1;", std::to_string(id));
            if (!row.size())
            {
                throw drogon::orm::UnexpectedRows("0 row found");
            }
            course = Model(row.at(0));
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return course;
    }
    drogon::Task<Model> createSql(drogon::orm::Transaction *tranPtr, Model course)
    {
        Model courseReturn;
        try
        {
            bool isInsert = false;
            auto sql = course.sqlForInserting(isInsert);
            auto result = co_await tranPtr->execSqlCoro(sql, course.getValueOfCode(), course.getValueOfName(), course.getValueOfCreatedDate(), course.getValueOfInstructorId());
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return courseReturn;
    }

    drogon::Task<bool> updateSql(Model course)
    {
        int idPrimaryKey = *course.getId();
        Model courseExist;
        bool result;
        try
        {
            auto db = drogon::app().getDbClient();
            drogon::orm::CoroMapper<Model> mp(db);
            courseExist = co_await mp.findByPrimaryKey(idPrimaryKey);
            result = co_await mp.update(course);
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return result;
    }

    drogon::Task<bool> removeSql(int id)
    {
        bool result;
        try
        {
            auto db = drogon::app().getDbClient();
            drogon::orm::CoroMapper<Model> mp(db);
            result = co_await mp.deleteBy(drogon::orm::Criteria(Model::Cols::_id, id));
            if (!result)
            {
                throw drogon::orm::UnexpectedRows("0 row found!");
            }
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return result;
    }

}