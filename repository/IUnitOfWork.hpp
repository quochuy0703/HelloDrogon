#pragma once
#include <iostream>
#include <memory>
#include <drogon/drogon.h>
#include "UserMicroserviceRepository.hpp"
#include "PcPartRepository.hpp"
#include "SystemSerialDetailRepository.hpp"

namespace app_repositories::unitofwork
{
    class IUnitOfWork
    {
    public:
        virtual drogon::Task<> BeginTransaction() = 0;
        virtual void Commit() = 0;
        virtual void Rollback() = 0;
        virtual std::shared_ptr<UserMicroserviceRepository> UserMicroservices() = 0;
        virtual std::shared_ptr<PcPartRepository> PcPartRepositorys() = 0;
        virtual std::shared_ptr<SystemSerialDetailRepository> SystemSerialDetailRepositories() = 0;
    };

}
