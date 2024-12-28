#pragma once
#include "IRepository.hpp"
#include "../models/PcPart.h"
#include <memory>
#include <string>

using Model = drogon_model::test::PcPart;

namespace app_repositories
{
    class PcPartRepository : public IRepository<Model>
    {
    public:
        PcPartRepository(std::shared_ptr<drogon::orm::Transaction> tranPtr); // Sử dụng transaction.
        ~PcPartRepository();

        drogon::Task<Model> GetById(std::string id);
        drogon::Task<std::vector<Model>> GetAll();
        drogon::Task<Model> Add(const Model &entity);

    private:
        std::shared_ptr<drogon::orm::Transaction> tranPtr_;
    };

}
