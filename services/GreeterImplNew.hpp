#pragma once
#include <memory>
#include "IGreeterNew.hpp"
#include "../repository/IWriterNew.hpp"
#include "../repository/IUnitOfWork.hpp"
#include "../models/UserMicroservice.h"

using namespace app_repositories;
using UserMicroserviceModel = drogon_model::test::UserMicroservice;

namespace app_services
{
    class GreeterImplNew : public IGreeterNew
    {
    private:
        // std::shared_ptr<IWriterNew> writer;
        std::shared_ptr<unitofwork::IUnitOfWork> uow;

    public:
        // Like "GreeterImpl(Writer* writer) {...}" but also marks this constructor
        // as the one to use for injection.
        // GreeterImplNew(std::shared_ptr<IWriterNew> writer) : writer(writer)
        // {
        //     std::cout << "Init GreeterImpl" << std::endl;
        // }

        GreeterImplNew(std::shared_ptr<unitofwork::IUnitOfWork> uow) : uow(uow)
        {
            std::cout << "Init GreeterImpl" << std::endl;
        }

        ~GreeterImplNew();

        virtual drogon::Task<UserMicroserviceDto> greet(const UserMicroserviceDto &data, bool useTransaction = true) override;
    };
}