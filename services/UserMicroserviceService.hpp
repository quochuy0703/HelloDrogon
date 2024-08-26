#pragma once

#include "../plugins/DIPlugin.h"
#include "../repository/IUnitOfWork.hpp"
#include "../repository/UnitOfWork.hpp"
#include <memory>
#include <iostream>
#include <iostream>
#include <drogon/drogon.h>
#include "../models/UserMicroservice.h"
#include "../repository/UserMicroserviceRepository.hpp"
#include "../dto/UserMicroserviceDto.hpp"
#include "../utils/Utils.hpp"
#include <trantor/utils/Date.h>
#include "IUserMicroserviceService.hpp"

using namespace drogon;

using UserMicroserviceDto = app_dto::userservice_dto::UserMicroserviceDto;
using UserMicroserviceModel = drogon_model::test::UserMicroservice;

using namespace app_repositories;
namespace app_services
{
    class UserMicroserviceService : public IUserMicroserviceService
    {
    public:
        UserMicroserviceService(std::shared_ptr<unitofwork::IUnitOfWork> uow) : uow(uow)
        {
            LOG_INFO << "Init UserMicroserviceService";
        }
        ~UserMicroserviceService()
        {
            LOG_INFO << "Destructor UserMicroserviceService";
        }

        drogon::Task<UserMicroserviceDto> CreateUser(const UserMicroserviceDto &data, bool useTransaction = true) override
        {

            UserMicroserviceModel user = UserMicroserviceDto::toModel(data);
            user.setCreatedat(::trantor::Date().now());
            try
            {
                if (useTransaction)
                {
                    co_await uow->BeginTransaction();
                }
                // auto *DIPluginPtr = app().getPlugin<drogon::plugin::DIPlugin>();

                // auto unit = DIPluginPtr->get<unitofwork::IUnitOfWork>();

                user = co_await uow->UserMicroservices()->Add(user);

                if (useTransaction)
                {
                    uow->Commit();
                }
            }
            catch (const std::exception &ex)
            {
                LOG_ERROR << ex.what();
                if (useTransaction)
                {
                    uow->Rollback();
                }
                throw std::runtime_error(ex.what());
            }

            co_return UserMicroserviceDto::fromModel(user);
        }

    private:
        std::shared_ptr<unitofwork::IUnitOfWork> uow;
    };

}
