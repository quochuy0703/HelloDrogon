#include <iostream>
#include <drogon/drogon.h>
#include "models/Course.h"

using Model = drogon_model::drogon_test::Course;

namespace app_repositories::course_repository
{
    drogon::Task<std::vector<Model>> getAllSql(std::shared_ptr<drogon::orm::Transaction> tranPtr);
    drogon::Task<std::vector<Model>> getAllSqlMapper(std::shared_ptr<drogon::orm::Transaction> tranPtr);

    drogon::Task<Model> getByIdSql(std::shared_ptr<drogon::orm::Transaction> tranPtr, int id);
    drogon::Task<Model> getByIdSqlMapper(std::shared_ptr<drogon::orm::Transaction> tranPtr, int id);

    drogon::Task<Model> createSql(std::shared_ptr<drogon::orm::Transaction> tranPtr, Model course);
    drogon::Task<Model> createSqlMapper(std::shared_ptr<drogon::orm::Transaction> tranPtr, Model course);

    drogon::Task<bool> updateSql(std::shared_ptr<drogon::orm::Transaction> tranPtr, Model course);
    drogon::Task<bool> updateSqlMapper(std::shared_ptr<drogon::orm::Transaction> tranPtr, Model course);

    drogon::Task<bool> removeSql(std::shared_ptr<drogon::orm::Transaction> tranPtr, int id);
    drogon::Task<bool> removeSqlMapper(std::shared_ptr<drogon::orm::Transaction> tranPtr, int id);

}