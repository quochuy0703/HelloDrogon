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

}