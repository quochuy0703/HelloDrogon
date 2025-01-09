#pragma once

#include <drogon/drogon.h>
#include <iostream>
#include <vector>
#include "../dto/SystemSerialDetailDto.hpp"
#include "../dto/models/SystemSerialDetailModel.hpp"

namespace app_services
{

    class ISystemSerialDetailService
    {
    public:
        using Dto = app_dto::SystemSerialDetailDto;
        using DtoModel = app_dto::models::SystemSerialDetailModel;
        virtual drogon::Task<std::vector<Dto>> GetAll(bool useTransaction = true) = 0;
        virtual drogon::Task<std::vector<DtoModel>> GetAllDto(bool useTransaction = true) = 0;
    };

}
