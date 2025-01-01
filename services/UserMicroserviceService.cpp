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

    drogon::Task<std::vector<UserMicroserviceDto>> UserMicroserviceService::GetAll(bool useTransaction)
    {
        std::vector<UserMicroserviceDto> lists;

        try
        {
            if (useTransaction)
            {
                co_await uow->BeginTransaction();
            }

            auto models = co_await uow->UserMicroservices()->GetAll();

            for (auto model : models)
            {
                lists.push_back(UserMicroserviceDto::fromModel(model));
            }

            if (useTransaction)
            {
                // uow->Commit();
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

        co_return lists;
    }
}
