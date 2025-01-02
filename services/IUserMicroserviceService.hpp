#pragma once

#include <drogon/drogon.h>
#include "../dto/UserMicroserviceDto.hpp"

using UserMicroserviceDto = app_dto::userservice_dto::UserMicroserviceDto;

namespace app_services
{
    class IUserMicroserviceService
    {
    public:
        virtual drogon::Task<UserMicroserviceDto> CreateUser(const UserMicroserviceDto &data, bool useTransaction) = 0;
        virtual drogon::Task<std::vector<UserMicroserviceDto>> GetAll(bool useTransaction = true)= 0;
    };

}
