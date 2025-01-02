#pragma once

// #include "../plugins/DIPlugin.h" //--> include chỗ này làm không compile được ???? tại sao lại include?? gây lỗi: error: 'UserMicroserviceService' is not a member of 'app_services'; did you mean 'IUserMicroserviceService'?
#include "../repository/IUnitOfWork.hpp"
#include <memory>
#include <iostream>
#include <drogon/drogon.h>
#include "../models/UserMicroservice.h"
#include "../services/UserMicroserviceService.hpp"
#include "../services/PcPartService.hpp"
#include "../dto/UserMicroserviceDto.hpp"
#include "../utils/Utils.hpp"
#include <trantor/utils/Date.h>
#include "IFatService.hpp"

#include <fruit/fruit.h>

using namespace drogon;

// using fruit::Component;
// using fruit::Injector;

using UserMicroserviceDto = app_dto::userservice_dto::UserMicroserviceDto;
using UserMicroserviceModel = drogon_model::test::UserMicroservice;

using namespace app_repositories;
namespace app_services
{
    class FatService : public IFatService
    {
    public:
        INJECT(FatService(std::shared_ptr<unitofwork::IUnitOfWork> uow, std::shared_ptr<PcPartService> pcPartService, std::shared_ptr<UserMicroserviceService> userMicroserviceService)) : uow(uow), pcPartService(pcPartService), userMicroserviceService(userMicroserviceService)
        {
            LOG_INFO << "Init FatService";
        }
        ~FatService()
        {
            LOG_INFO << "Destructor FatService";
        }

        virtual drogon::Task<bool> getAll() override;

    private:
        std::shared_ptr<unitofwork::IUnitOfWork> uow;
        std::shared_ptr<PcPartService> pcPartService;
        std::shared_ptr<UserMicroserviceService> userMicroserviceService;
    };

}
