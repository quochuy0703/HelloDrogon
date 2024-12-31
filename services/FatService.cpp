#include "FatService.hpp"

drogon::Task<bool> app_services::FatService::getAll()
{

    try
    {
        co_await uow->BeginTransaction();

        auto modelPart = co_await pcPartService->GetAll();
        auto modelUser = co_await this->userMicroserviceService->GetAll();
    }
    catch (const std::exception &ex)
    {
        LOG_ERROR << ex.what();

        uow->Rollback();

        throw std::runtime_error(ex.what());
    }
    co_return true;
}