#pragma once
#include <memory>
#include <drogon/drogon.h>
#include "../models/UserMicroservice.h"
#include "../dto/UserMicroserviceDto.hpp"

using UserMicroserviceDto = app_dto::userservice_dto::UserMicroserviceDto;
using UserMicroserviceModel = drogon_model::test::UserMicroservice;

namespace app_services
{
    class IUserMicroserviceService
    {
    public:
        virtual ~IUserMicroserviceService() = default;

        virtual drogon::Task<UserMicroserviceDto> CreateUser(const UserMicroserviceDto &data, bool useTransaction) = 0;
    };

}
