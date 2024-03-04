#include "UserService.hpp"
#include "../utils/Utils.hpp"
#include "../repository/UserRepository.hpp"

using UserModel = drogon_model::test::UserLogin;

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

    drogon::Task<app_dto::user::UserDto> getById(int id)
    {
        UserModel user = co_await app_repositories::user_repository::getById(id);
        co_return app_dto::user::UserDto::fromUser(user);
    }

    drogon::Task<app_dto::user::UserDto> create(app_dto::user::UserDto data)
    {
        }

}