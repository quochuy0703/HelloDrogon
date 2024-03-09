#include <iostream>
#include <drogon/drogon.h>
#include "models/Course.h"
#include "../repository/CourseSqlRepository.hpp"
#include "../dto/CourseDto.cpp"
#include "../utils/Utils.hpp"
#include <trantor/utils/Date.h>

using CourseDto = app_dto::course::CourseDto;
using CourseModel = drogon_model::test::Course;

namespace app_services::course_service
{
    drogon::Task<std::vector<CourseDto>> getAllSql();

    drogon::Task<CourseDto> getByIdSql(int id);

    drogon::Task<CourseDto> createSql(CourseDto data);

    // drogon::Task<bool> update(CourseDto data);

    // drogon::Task<bool> remove(int id);
}