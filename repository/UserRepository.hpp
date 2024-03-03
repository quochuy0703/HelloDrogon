#include <iostream>
#include <drogon/drogon.h>
#include "models/User.h"

namespace app_repositories::user_repository
{
    drogon::Task<std::vector<drogon_model::drogon_test::User>> getAll();
    drogon::Task<drogon_model::drogon_test::User> getById(int id);
    // drogon::Task<drogon_model::drogon_test::User> create();

    // drogon::Task<drogon_model::drogon_test::User> update();

    // drogon::Task<drogon_model::drogon_test::User> remove();

}