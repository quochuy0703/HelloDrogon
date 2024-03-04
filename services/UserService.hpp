#include <iostream>
#include <drogon/drogon.h>
#include "models/UserLogin.h"
#include "../repository/UserRepository.hpp"
#include "../dto/UserDto.cpp"

namespace app_services::user
{
    drogon::Task<std::vector<app_dto::user::UserDto>> getAll();
    drogon::Task<app_dto::user::UserDto> getById(int id);
    drogon::Task<app_dto::user::UserDto> create(app_dto::user::UserDto data);

    // drogon::Task<drogon_model::drogon_test::User> update();

    // drogon::Task<drogon_model::drogon_test::User> remove();
}