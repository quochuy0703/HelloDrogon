#include "UserRepository.hpp"
#include "../utils/Utils.hpp"

using UserModel = drogon_model::drogon_test::User;

namespace app_repositories::user_repository
{

    drogon::Task<std::vector<drogon_model::drogon_test::User>> getAll()
    {

        std::vector<UserModel> users;
        try
        {
            auto db = drogon::app().getDbClient();
            drogon::orm::CoroMapper<UserModel> usr(db);
            users = co_await usr.findAll();
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
        }

        co_return users;
    }

    drogon::Task<std::vector<drogon_model::drogon_test::User>> getAllBySql()
    {
        std::vector<UserModel> users;
        try
        {
            auto db = drogon::app().getDbClient();
            auto rows = co_await db->execSqlCoro("SELECT * from user_login;");
            for (auto row : rows)
            {
                users.push_back(UserModel(row));
            }
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
        }

        co_return users;
    }

    drogon::Task<std::vector<drogon_model::drogon_test::User>> getByCondition(std::map<std::string, std::string> condition)
    {
        std::vector<UserModel> users;
        try
        {
            drogon::orm::Criteria criteria;
            if (condition["id"].compare("") != 0)
            {
                criteria = criteria && drogon::orm::Criteria(UserModel::Cols::_id, condition["id"]);
            }
            if (condition["email"].compare("") != 0)
            {
                criteria = criteria && drogon::orm::Criteria(UserModel::Cols::_email, condition["email"]);
            }
            if (condition["name"].compare("") != 0)
            {
                criteria = criteria && drogon::orm::Criteria(UserModel::Cols::_name, condition["name"]);
            }
            auto db = drogon::app().getDbClient();
            drogon::orm::CoroMapper<UserModel> usr(db);
            users = co_await usr.findBy(criteria);
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
        }

        co_return users;
    }

    drogon::Task<drogon_model::drogon_test::User> getById(int id)
    {

        UserModel user;

        try
        {
            auto db = drogon::app().getDbClient();
            drogon::orm::CoroMapper<UserModel> usr(db);
            user = co_await usr.findOne(drogon::orm::Criteria(UserModel::Cols::_id, id));
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
        }

        co_return user;
    }

    drogon::Task<drogon_model::drogon_test::User> create(drogon_model::drogon_test::User user)
    {

        UserModel userReturn;
        try
        {
            auto db = drogon::app().getDbClient();
            drogon::orm::CoroMapper<UserModel> usr(db);
            userReturn = co_await usr.insert(user);
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
        }

        co_return userReturn;
    }

    drogon::Task<bool> update(drogon_model::drogon_test::User user)
    {

        int idPrimaryKey = *user.getId();
        UserModel userExist;
        bool result;
        try
        {
            auto db = drogon::app().getDbClient();
            drogon::orm::CoroMapper<UserModel> usr(db);
            userExist = co_await usr.findByPrimaryKey(idPrimaryKey);
            result = co_await usr.update(user);
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
        }

        co_return result;
    }

    drogon::Task<bool> remove(int id)
    {

        bool result;
        try
        {
            auto db = drogon::app().getDbClient();
            drogon::orm::CoroMapper<UserModel> usr(db);
            result = co_await usr.deleteBy(drogon::orm::Criteria(UserModel::Cols::_id, id));
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
        }

        co_return result;
    }

}