#include <iostream>
#include <drogon/drogon.h>
#include "models/User.h"

namespace app_repositories::user_repository
{
    drogon::Task<std::vector<drogon_model::drogon_test::User>> getAll();
    drogon::Task<std::vector<drogon_model::drogon_test::User>> getAllBySql();
    drogon::Task<std::vector<drogon_model::drogon_test::User>> getByCondition(std::map<std::string, std::string> condition);
    drogon::Task<drogon_model::drogon_test::User> getById(int id);
    drogon::Task<drogon_model::drogon_test::User> create(drogon_model::drogon_test::User user);

    drogon::Task<bool> update(drogon_model::drogon_test::User user);

    drogon::Task<bool> remove(int id);

}