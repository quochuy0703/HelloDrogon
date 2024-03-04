#include "UserRepository.hpp"
#include "../utils/Utils.hpp"

using UserModel = drogon_model::test::UserLogin;

namespace app_repositories::user_repository
{

    drogon::Task<std::vector<drogon_model::test::UserLogin>> getAll()
    {
        auto db = drogon::app().getDbClient();
        std::vector<UserModel> users;
        try
        {
            drogon::orm::CoroMapper<UserModel> usr(db);
            users = co_await usr.findAll();
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
        }

        co_return users;
    }

    drogon::Task<drogon_model::test::UserLogin> getById(int id)
    {
        auto db = drogon::app().getDbClient();
        UserModel user;

        try
        {
            drogon::orm::CoroMapper<UserModel> usr(db);
            user = co_await usr.findOne(drogon::orm::Criteria(UserModel::Cols::_id, id));
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
        }

        co_return user;
    }
    drogon::Task<drogon_model::test::UserLogin> create(drogon_model::test::UserLogin user)
    {
        auto db = drogon::app().getDbClient();
        UserModel userReturn;
        try
        {
            drogon::orm::CoroMapper<UserModel> usr(db);
            userReturn = co_await usr.insert(user);
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
        }

        co_return userReturn;
    }

    drogon::Task<bool> update(drogon_model::test::UserLogin user)
    {
        auto db = drogon::app().getDbClient();
        int idPrimaryKey = *user.getId();
        UserModel userExist;
        bool result;
        try
        {
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

    drogon::Task<drogon_model::test::UserLogin> remove(int id)
    {
        auto db = drogon::app().getDbClient();
        bool result;
        try
        {
            drogon::orm::CoroMapper<UserModel> usr(db);
            result = co_await usr.deleteBy(drogon::orm::Criteria(UserModel::Cols::_id, id));
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
        }
    }

}