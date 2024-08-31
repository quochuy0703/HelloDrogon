#pragma once
#include <drogon/drogon.h>
#include "../dto/UserMicroserviceDto.hpp"

using UserMicroserviceDto = app_dto::userservice_dto::UserMicroserviceDto;
namespace app_services
{
    class IGreeterNew
    {
    public:
        virtual drogon::Task<UserMicroserviceDto> greet(const UserMicroserviceDto &data, bool useTransaction) = 0;
    };
}
