#pragma once

// #include "../plugins/DIPlugin.h" //--> include chỗ này làm không compile được ???? tại sao lại include?? gây lỗi: error: 'UserMicroserviceService' is not a member of 'app_services'; did you mean 'IUserMicroserviceService'?

#include <memory>
#include <iostream>
#include <drogon/drogon.h>
#include "../utils/Utils.hpp"
#include <trantor/utils/Date.h>
#include <fruit/fruit.h>

#include "../models/PcPart.h"
#include "../repository/SystemSerialDetailRepository.hpp"
#include "../services/ISystemSerialDetailService.hpp"
#include "../repository/IUnitOfWork.hpp"

using namespace drogon;

// using fruit::Component;
// using fruit::Injector;

using namespace app_repositories;
namespace app_services
{

    class SystemSerialDetailService : public ISystemSerialDetailService
    {
    public:
        using Model = drogon_model::smart::SystemSerialDetail;
        INJECT(SystemSerialDetailService(std::shared_ptr<unitofwork::IUnitOfWork> uow)) : uow(uow)
        {
            LOG_INFO << "Init SystemSerialDetailService";
        }
        ~SystemSerialDetailService()
        {
            LOG_INFO << "Destructor SystemSerialDetailService";
        }

        virtual drogon::Task<std::vector<ISystemSerialDetailService::Dto>> GetAll(bool useTransaction = true) override;
        virtual drogon::Task<std::vector<ISystemSerialDetailService::DtoModel>> GetAllDto(bool useTransaction = true) override;

    private:
        std::shared_ptr<unitofwork::IUnitOfWork> uow;
    };

}
