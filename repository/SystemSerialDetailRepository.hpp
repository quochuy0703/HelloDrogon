#pragma once

#include <iostream>
#include "IRepository.hpp"
#include "../models/SystemSerialDetail.h"
#include <memory>
#include <string>
#include "../utils/MapJson.hpp"
#include "../dto/models/SystemSerialDetailModel.hpp"

namespace app_repositories
{
    class SystemSerialDetailRepository : public IRepository<drogon_model::smart::SystemSerialDetail>
    {
    public:
        using Model = drogon_model::smart::SystemSerialDetail;
        SystemSerialDetailRepository(std::shared_ptr<drogon::orm::Transaction> tranPtr); // Sử dụng transaction.
        ~SystemSerialDetailRepository();

        drogon::Task<Model> GetById(std::string code) override;
        drogon::Task<std::vector<Model>> GetAll() override;
        drogon::Task<Model> Add(const Model &entity) override;

    private:
        std::shared_ptr<drogon::orm::Transaction> tranPtr_;
    };

}
