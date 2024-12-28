#pragma once
#include "IUnitOfWork.hpp"
#include <drogon/drogon.h>
#include "UserMicroserviceRepository.hpp"

using namespace app_repositories;

namespace app_repositories::unitofwork
{
    class UnitOfWork : public IUnitOfWork
    {
    public:
        // UnitOfWork(const drogon::orm::DbClientPtr &client);
        UnitOfWork()
        {
            LOG_INFO << "Init UnitOfWork";
            dbClient_ = drogon::app().getDbClient();
        };
        ~UnitOfWork();

        drogon::Task<> BeginTransaction() override;
        void Commit() override;
        void Rollback() override;
        std::shared_ptr<UserMicroserviceRepository> UserMicroservices() override;
        std::shared_ptr<PcPartRepository> PcPartRepositorys() override;

    private:
        drogon::orm::DbClientPtr dbClient_;
        std::shared_ptr<drogon::orm::Transaction> transaction_;
        std::shared_ptr<UserMicroserviceRepository> usermicroserviceRepository_;
        std::shared_ptr<PcPartRepository> pcPartRepository_;
    };
}
