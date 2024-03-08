#include "CourseRepository.hpp"

namespace app_repositories::course_repository
{
    drogon::Task<std::vector<Model>> getAll()
    {
        std::vector<Model> courses;
        try
        {
            auto db = drogon::app().getDbClient();
            drogon::orm::CoroMapper<Model> mp(db);
            courses = co_await mp.findAll();
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return courses;
    }
    drogon::Task<Model> getById(int id)
    {
        Model course;

        try
        {
            auto db = drogon::app().getDbClient();
            drogon::orm::CoroMapper<Model> mp(db);
            course = co_await mp.findOne(drogon::orm::Criteria(Model::Cols::_id, std::to_string(id)));
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return course;
    }
    drogon::Task<Model> getByIdSql(int id)
    {
        Model course;

        try
        {
            auto db = drogon::app().getDbClient();
            auto row = co_await db->execSqlCoro("SELECT * FROM course where id = $1;", std::to_string(id));
            course = Model(row.at(0));
            // course = co_await mp.findOne(drogon::orm::Criteria(Model::Cols::_id, id));
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return course;
    }

    drogon::Task<Model> create(Model course)
    {
        Model courseReturn;
        try
        {
            auto db = drogon::app().getDbClient();
            drogon::orm::CoroMapper<Model> mp(db);
            courseReturn = co_await mp.insert(course);
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return courseReturn;
    }

    drogon::Task<bool> update(Model course)
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

    drogon::Task<bool> remove(int id)
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