#include <iostream>
#include <drogon/drogon.h>
#include "models/UserMicroservice.h"

using Model = drogon_model::test::UserMicroservice;

namespace app_repositories::usermicroservice_repository
{
    drogon::Task<std::vector<Model>> getAll();
    drogon::Task<Model> getById(std::string id);
    // drogon::Task<Model> getByIdSql(int id);
    drogon::Task<Model> create(Model course);

    drogon::Task<bool> update(Model course);

    drogon::Task<bool> remove(std::string id);

}