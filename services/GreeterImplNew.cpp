#include <iostream>
#include "GreeterImplNew.hpp"

namespace app_services
{

    GreeterImplNew::~GreeterImplNew()
    {
        std::cout << "Destruct GreeterImpl" << std::endl;
    };

    drogon::Task<UserMicroserviceDto> GreeterImplNew::greet(const UserMicroserviceDto &data, bool useTransaction)
    {
        LOG_INFO << "Hello World";
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

}