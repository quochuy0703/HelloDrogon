#include "SystemSerialDetailService.hpp"

using namespace app_repositories;
namespace app_services
{

    drogon::Task<std::vector<ISystemSerialDetailService::Dto>> SystemSerialDetailService::GetAll(bool useTransaction)
    {

        std::vector<ISystemSerialDetailService::Dto> lists;

        try
        {
            if (useTransaction)
            {
                co_await uow->BeginTransaction();
            }

            auto models = co_await uow->SystemSerialDetailRepositories()->GetAll();

            for (auto model : models)
            {
                lists.push_back(SystemSerialDetailService::Dto::fromModel(model));
            }

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

        co_return lists;
    }

}
