#pragma once
#include "IRepository.hpp"
#include "../models/SystemSerialDetail.h"
#include <memory>
#include <string>

namespace app_repositories
{
    class SystemSerialDetailRepository : public IRepository<drogon_model::smart::SystemSerialDetail>
    {
    public:
        using Model = drogon_model::smart::SystemSerialDetail;
        SystemSerialDetailRepository(std::shared_ptr<drogon::orm::Transaction> tranPtr); // Sử dụng transaction.
        ~SystemSerialDetailRepository();

        drogon::Task<Model> getById(std::string code);
        drogon::Task<std::vector<Model>> getAll();
        drogon::Task<Model> add(const Model &entity);

    private:
        std::shared_ptr<drogon::orm::Transaction> tranPtr_;
    };

}
