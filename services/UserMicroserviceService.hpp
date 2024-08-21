#include <iostream>
#include <drogon/drogon.h>
#include "models/UserMicroservice.h"
#include "../repository/UserMicroserviceRepository.hpp"
#include "../dto/UserMicroserviceDto.cpp"
#include "../utils/Utils.hpp"
#include <trantor/utils/Date.h>

using UserMicroserviceDto = app_dto::userservice_dto::UserMicroserviceDto;
using UserMicroserviceModel = drogon_model::test::UserMicroservice;

namespace app_services::usermicroservice_service
{
    drogon::Task<std::vector<UserMicroserviceDto>> getAll();

    drogon::Task<UserMicroserviceDto> getById(std::string id);

    drogon::Task<UserMicroserviceDto> create(UserMicroserviceDto data);

    drogon::Task<bool> update(UserMicroserviceDto data);

    drogon::Task<bool> remove(std::string id);
}