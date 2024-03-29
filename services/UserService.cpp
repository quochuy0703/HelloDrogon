#include "UserService.hpp"
#include "../utils/Utils.hpp"

using UserModel = drogon_model::drogon_test::User;

namespace app_services::user
{

    drogon::Task<std::vector<app_dto::user::UserDto>> getAll()
    {

        std::vector<UserModel> users = co_await app_repositories::user_repository::getAll();

        std::vector<app_dto::user::UserDto> userDtos;
        for (auto user : users)
        {
            app_dto::user::UserDto temp = app_dto::user::UserDto::fromUser(user);
            userDtos.push_back(temp);
        }
        co_return userDtos;
    }

    drogon::Task<std::vector<app_dto::user::UserDto>> getAllBySql()
    {

        std::vector<UserModel> users = co_await app_repositories::user_repository::getAllBySql();

        std::vector<app_dto::user::UserDto> userDtos;
        for (auto user : users)
        {
            app_dto::user::UserDto temp = app_dto::user::UserDto::fromUser(user);
            userDtos.push_back(temp);
        }
        co_return userDtos;
    }

    drogon::Task<std::vector<app_dto::user::UserDto>> getByCondition(std::map<std::string, std::string> condition)
    {
        std::vector<UserModel> users = co_await app_repositories::user_repository::getByCondition(condition);

        std::vector<app_dto::user::UserDto> userDtos;
        for (auto user : users)
        {
            app_dto::user::UserDto temp = app_dto::user::UserDto::fromUser(user);
            userDtos.push_back(temp);
        }
        co_return userDtos;
    }

    drogon::Task<app_dto::user::UserDto> getById(int id)
    {
        UserModel user = co_await app_repositories::user_repository::getById(id);
        co_return app_dto::user::UserDto::fromUser(user);
    }

    drogon::Task<app_dto::user::UserDto> create(app_dto::user::UserDto data)
    {

        UserModel user = app_dto::user::UserDto::toUser(data);
        user = co_await app_repositories::user_repository::create(user);
        co_return app_dto::user::UserDto::fromUser(user);
    }

    drogon::Task<bool> update(app_dto::user::UserDto data)
    {
        UserModel user = app_dto::user::UserDto::toUser(data);
        bool result = co_await app_repositories::user_repository::update(user);
        co_return result;
    }

    drogon::Task<bool> remove(int id)
    {
        bool result = co_await app_repositories::user_repository::remove(id);
        co_return result;
    }
}
