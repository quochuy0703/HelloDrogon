#include <iostream>
#include <drogon/drogon.h>
#include "models/Course.h"

using Model = drogon_model::drogon_test::Course;

namespace app_repositories::course_repository
{
    drogon::Task<std::vector<Model>> getAll();
    drogon::Task<Model> getById(int id);
    drogon::Task<Model> create(Model course);

    drogon::Task<bool> update(Model course);

    drogon::Task<bool> remove(int id);

}