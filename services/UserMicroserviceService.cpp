#include "UserMicroserviceService.hpp"

using namespace app_repositories;
namespace app_services
{

    drogon::Task<UserMicroserviceDto> UserMicroserviceService::CreateUser(const UserMicroserviceDto &data, bool useTransaction)
    {

        UserMicroserviceModel user = UserMicroserviceDto::toModel(data);
        user.setCreatedat(::trantor::Date().now());
        try
        {
            if (useTransaction)
            {
                co_await uow->BeginTransaction();
            }

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
