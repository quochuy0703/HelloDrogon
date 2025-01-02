#pragma once

// #include "../plugins/DIPlugin.h" //--> include chỗ này làm không compile được ???? tại sao lại include?? gây lỗi: error: 'UserMicroserviceService' is not a member of 'app_services'; did you mean 'IUserMicroserviceService'?
#include "../repository/IUnitOfWork.hpp"
#include <memory>
#include <iostream>
#include <drogon/drogon.h>
#include "../models/UserMicroservice.h"
#include "../repository/UserMicroserviceRepository.hpp"
#include "../dto/UserMicroserviceDto.hpp"
#include "../utils/Utils.hpp"
#include <trantor/utils/Date.h>
#include "IUserMicroserviceService.hpp"
#include <fruit/fruit.h>

using namespace drogon;

// using fruit::Component;
// using fruit::Injector;

using UserMicroserviceDto = app_dto::userservice_dto::UserMicroserviceDto;
using UserMicroserviceModel = drogon_model::test::UserMicroservice;

using namespace app_repositories;
namespace app_services
{
    class UserMicroserviceService : public IUserMicroserviceService
    {
    public:
        INJECT(UserMicroserviceService(std::shared_ptr<unitofwork::IUnitOfWork> uow)): uow(std::move(uow))
        {
            LOG_INFO << "Init UserMicroserviceService";
        }
        ~UserMicroserviceService()
        {
            LOG_INFO << "Destructor UserMicroserviceService";
        }

        virtual drogon::Task<UserMicroserviceDto> CreateUser(const UserMicroserviceDto &data, bool useTransaction = true) override;
        virtual drogon::Task<std::vector<UserMicroserviceDto>> GetAll(bool useTransaction = true) override;

    private:
        std::shared_ptr<unitofwork::IUnitOfWork> uow;
    };

}
