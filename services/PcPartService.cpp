#include "PcPartService.hpp"

using namespace app_repositories;
namespace app_services
{

    drogon::Task<std::vector<Dto>> PcPartService::GetAll(bool useTransaction)
    {

        std::vector<Dto> lists;

        try
        {
            if (useTransaction)
            {
                co_await uow->BeginTransaction();
            }

            auto models = co_await uow->PcPartRepositorys()->GetAll();

            for (auto model : models)
            {
                lists.push_back(Dto::fromModel(model));
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
