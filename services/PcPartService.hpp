#pragma once

// #include "../plugins/DIPlugin.h" //--> include chỗ này làm không compile được ???? tại sao lại include?? gây lỗi: error: 'UserMicroserviceService' is not a member of 'app_services'; did you mean 'IUserMicroserviceService'?
#include "../repository/IUnitOfWork.hpp"
#include <memory>
#include <iostream>
#include <drogon/drogon.h>
#include "../models/PcPart.h"
#include "../repository/PcPartRepository.hpp"
#include "../utils/Utils.hpp"
#include <trantor/utils/Date.h>
#include "IPcPartService.hpp"
// #include <fruit/fruit.h>

using namespace drogon;

// using fruit::Component;
// using fruit::Injector;

using UserMicroserviceModel = drogon_model::test::UserMicroservice;

using namespace app_repositories;
namespace app_services
{
    class PcPartService : public IPcPartService
    {
    public:
        PcPartService(std::shared_ptr<unitofwork::IUnitOfWork> uow) : uow(uow)
        {
            LOG_INFO << "Init UserMicroserviceService";
        }
        ~PcPartService()
        {
            LOG_INFO << "Destructor UserMicroserviceService";
        }

        virtual drogon::Task<std::vector<Dto>> GetAll(bool useTransaction = true) override;

    private:
        std::shared_ptr<unitofwork::IUnitOfWork> uow;
    };

}
