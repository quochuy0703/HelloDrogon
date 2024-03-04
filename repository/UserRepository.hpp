#include <iostream>
#include <drogon/drogon.h>
#include "models/UserLogin.h"

namespace app_repositories::user_repository
{
    drogon::Task<std::vector<drogon_model::test::UserLogin>> getAll();
    drogon::Task<drogon_model::test::UserLogin> getById(int id);
    drogon::Task<drogon_model::test::UserLogin> create(drogon_model::test::UserLogin user);

    drogon::Task<bool> update(drogon_model::test::UserLogin user);

    drogon::Task<drogon_model::test::UserLogin> remove(int id);

}