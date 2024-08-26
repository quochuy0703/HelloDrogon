#pragma once
#include "IUnitOfWork.hpp"
#include <drogon/drogon.h>
#include "UserMicroserviceRepository.hpp"

namespace app_repositories::unitofwork
{
    class UnitOfWork : public IUnitOfWork
    {
    public:
        // UnitOfWork(const drogon::orm::DbClientPtr &client);
        UnitOfWork();
        ~UnitOfWork();

        drogon::Task<> BeginTransaction() override;
        void Commit() override;
        void Rollback() override;
        std::shared_ptr<UserMicroserviceRepository> UserMicroservices();

    private:
        drogon::orm::DbClientPtr dbClient_;
        std::shared_ptr<drogon::orm::Transaction> transaction_;
        std::shared_ptr<UserMicroserviceRepository> usermicroserviceRepository_;
    };
}
