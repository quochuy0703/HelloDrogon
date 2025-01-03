#pragma once
#include "IUnitOfWork.hpp"
#include <drogon/drogon.h>

#include <fruit/fruit.h>

using namespace app_repositories;

namespace app_repositories::unitofwork
{
    class UnitOfWork : public IUnitOfWork
    {
    public:
        // UnitOfWork(const drogon::orm::DbClientPtr &client);
        INJECT(UnitOfWork());
        ~UnitOfWork();

        drogon::Task<> BeginTransaction() override;
        void Commit() override;
        void Rollback() override;
        std::shared_ptr<UserMicroserviceRepository> UserMicroservices() override;
        std::shared_ptr<PcPartRepository> PcPartRepositorys() override;
        std::shared_ptr<SystemSerialDetailRepository> SystemSerialDetailRepositories() override;

    private:
        drogon::orm::DbClientPtr dbClient_;
        drogon::orm::DbClientPtr dbClientSmart_;
        std::shared_ptr<drogon::orm::Transaction> transaction_;
        std::shared_ptr<drogon::orm::Transaction> transactionSmart_;
        std::shared_ptr<UserMicroserviceRepository> usermicroserviceRepository_;
        std::shared_ptr<PcPartRepository> pcPartRepository_;
        std::shared_ptr<SystemSerialDetailRepository> systemSerialDetailRepository_;
    };
}
