#include "CourseRepository.hpp"

namespace app_repositories::course_repository
{
    drogon::Task<std::vector<Model>> getAllSql(std::shared_ptr<drogon::orm::Transaction> tranPtr)
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

    drogon::Task<std::vector<Model>> getAllSqlMapper(std::shared_ptr<drogon::orm::Transaction> tranPtr)
    {
        std::vector<Model> courses;

        try
        {
            drogon::orm::CoroMapper<Model> mp(tranPtr);
            courses = co_await mp.findAll();
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
            auto row = co_await tranPtr->execSqlCoro("SELECT * FROM course where id = $1", std::to_string(id));
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

    drogon::Task<Model> getByIdSqlMapper(std::shared_ptr<drogon::orm::Transaction> tranPtr, int id)
    {
        Model course;

        try
        {
            drogon::orm::CoroMapper<Model> mp(tranPtr);
            course = co_await mp.findOne(drogon::orm::Criteria(Model::Cols::_id, id));
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return course;
    }

    drogon::Task<Model> createSql(std::shared_ptr<drogon::orm::Transaction> tranPtr, Model course)
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

    drogon::Task<Model> createSqlMapper(std::shared_ptr<drogon::orm::Transaction> tranPtr, Model course)
    {
        Model courseReturn;
        try
        {
            drogon::orm::CoroMapper<Model> mp(tranPtr);
            courseReturn = co_await mp.insert(course);
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return courseReturn;
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

    drogon::Task<bool> updateSqlMapper(std::shared_ptr<drogon::orm::Transaction> tranPtr, Model course)
    {
        int idPrimaryKey = *course.getId();
        bool result;
        try
        {
            drogon::orm::CoroMapper<Model> mp(tranPtr);
            result = co_await mp.update(course);
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

    drogon::Task<bool> removeSqlMapper(std::shared_ptr<drogon::orm::Transaction> tranPtr, int id)
    {
        bool result;
        try
        {
            drogon::orm::CoroMapper<Model> mp(tranPtr);
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