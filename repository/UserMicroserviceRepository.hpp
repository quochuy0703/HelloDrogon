#pragma once
#include "IRepository.hpp"
#include "../models/UserMicroservice.h"
#include <memory>
#include <string>

using UserMicroserviceModel = drogon_model::test::UserMicroservice;
using namespace app_repositories;
namespace app_repositories
{
    class UserMicroserviceRepository : public IRepository<UserMicroserviceModel>
    {
    public:
        UserMicroserviceRepository(std::shared_ptr<drogon::orm::Transaction> tranPtr); // Sử dụng transaction.
        ~UserMicroserviceRepository();

        drogon::Task<UserMicroserviceModel> GetById(std::string id);
        drogon::Task<std::vector<UserMicroserviceModel>> GetAll();
        drogon::Task<UserMicroserviceModel> Add(const UserMicroserviceModel &entity);

    private:
        std::shared_ptr<drogon::orm::Transaction> tranPtr_;
    };

}
