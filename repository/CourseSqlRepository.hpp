#include <iostream>
#include <drogon/drogon.h>
#include "models/Course.h"

using Model = drogon_model::test::Course;

namespace app_repositories::course_repository
{
    drogon::Task<std::vector<Model>> getAllSql(drogon::orm::Transaction *tranPtr);
    drogon::Task<Model> getByIdSql(std::shared_ptr<drogon::orm::Transaction> tranPtr, int id);
    drogon::Task<Model> createSql(drogon::orm::Transaction *tranPtr, Model course);

    drogon::Task<bool> updateSql(std::shared_ptr<drogon::orm::Transaction> tranPtr, Model course);

    drogon::Task<bool> removeSql(std::shared_ptr<drogon::orm::Transaction> tranPtr, int id);

}