#include <iostream>
#include <drogon/drogon.h>
#include "models/Course.h"
#include "../repository/CourseSqlRepository.hpp"
#include "../dto/CourseDto.cpp"
#include "../utils/Utils.hpp"
#include <trantor/utils/Date.h>

using CourseDto = app_dto::course::CourseDto;
using CourseModel = drogon_model::drogon_test::Course;

namespace app_services::course_service
{
    drogon::Task<std::vector<CourseDto>> getAllSql();

    drogon::Task<std::vector<CourseDto>> getByCondition(std::map<std::string, std::string> condition);

    drogon::Task<CourseDto> getByIdSql(int id);

    drogon::Task<CourseDto> createSql(CourseDto data);

    drogon::Task<bool> updateSql(CourseDto data);

    drogon::Task<bool> removeSql(int id);
}