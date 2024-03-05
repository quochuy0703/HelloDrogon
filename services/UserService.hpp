#include <iostream>
#include <drogon/drogon.h>
#include "models/UserLogin.h"
#include "../repository/UserRepository.hpp"
#include "../dto/UserDto.cpp"

namespace app_services::user
{
    drogon::Task<std::vector<app_dto::user::UserDto>> getAll();
    drogon::Task<std::vector<app_dto::user::UserDto>> getAllBySql();
    drogon::Task<std::vector<app_dto::user::UserDto>> getByCondition(std::map<std::string, std::string> condition);

    drogon::Task<app_dto::user::UserDto> getById(int id);

    drogon::Task<app_dto::user::UserDto> create(app_dto::user::UserDto data);

    drogon::Task<bool> update(app_dto::user::UserDto data);

    drogon::Task<bool> remove(int id);
}