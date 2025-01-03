#pragma once

#include <drogon/drogon.h>
#include <iostream>
#include <vector>
#include "../dto/SystemSerialDetailDto.hpp"

namespace app_services
{

    class ISystemSerialDetailService
    {
    public:
        using Dto = app_dto::SystemSerialDetailDto;
        virtual drogon::Task<std::vector<Dto>> GetAll(bool useTransaction = true) = 0;
    };

}
