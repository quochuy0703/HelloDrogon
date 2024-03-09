#include <iostream>
#include <drogon/drogon.h>
#include "models/Course.h"

using Model = drogon_model::drogon_test::Course;

namespace app_repositories::course_repository
{
    drogon::Task<std::vector<Model>> getAllSql(drogon::orm::Transaction *tranPtr);
    drogon::Task<Model> getByIdSql(drogon::orm::Transaction *tranPtr, int id);
    drogon::Task<Model> createSql(drogon::orm::Transaction *tranPtr, Model course);

    drogon::Task<bool> updateSql(Model course);

    drogon::Task<bool> removeSql(int id);

}