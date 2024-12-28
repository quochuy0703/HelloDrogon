#pragma once

#include <drogon/drogon.h>
#include <iostream>
#include <vector>
#include "../dto/PcPartDto.hpp"

using Dto = app_dto::PcPartDto;

namespace app_services
{
    class IPcPartService
    {
    public:
        virtual drogon::Task<std::vector<Dto>> GetAll(bool useTransaction = true) = 0;
    };

}
