#include "CourseRepository.hpp"

namespace app_repositories::course_repository
{
    drogon::Task<std::vector<Model>> getAllSql(drogon::orm::Transaction *tranPtr)
    {
        std::vector<Model> courses;
        try
        {
            auto rows = co_await tranPtr->execSqlCoro("SELECT * FROM course;");
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

    drogon::Task<Model> getByIdSql(std::shared_ptr<drogon::orm::Transaction> tranPtr, int id)
    {
        Model course;

        try
        {
            auto row = co_await tranPtr->execSqlCoro("SELECT * FROM course where id = $1;", std::to_string(id));
            if (row.size() == 0)
            {
                throw drogon::orm::UnexpectedRows("O row found!");
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
            bool sel = false;
            auto sql = course.sqlForInserting(sel);
            LOG_INFO << sql;

            auto row = co_await tranPtr->execSqlCoro(sql, *course.getCreatedDate(), *course.getCode(), *course.getName(), std::to_string(*course.getInstructorId()));
            LOG_INFO << row.affectedRows();
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return course;
    }

    drogon::Task<bool> updateSql(std::shared_ptr<drogon::orm::Transaction> tranPtr, Model course)
    {
        int idPrimaryKey = *course.getId();
        Model courseExist;
        bool result;
        try
        {
            auto row = co_await tranPtr->execSqlCoro("UPDATE course SET code = $1, name = $2, last_modified_date = $3  where id = $4;", course.getValueOfCode(), course.getValueOfName(), course.getValueOfLastModifiedDate(), course.getValueOfId());
            LOG_INFO << row.affectedRows();
            result = row.affectedRows();
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return result;
    }

    drogon::Task<bool> removeSql(std::shared_ptr<drogon::orm::Transaction> tranPtr, int id)
    {
        bool result;
        try
        {
            auto row = co_await tranPtr->execSqlCoro("DELETE FROM course where id = $1;", std::to_string(id));
            LOG_INFO << row.affectedRows();
            result = row.affectedRows();
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