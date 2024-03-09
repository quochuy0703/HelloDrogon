#pragma once

#include <iostream>
#include <optional>
#include <drogon/drogon.h>
#include "../models/Course.h"

using CourseModel = drogon_model::drogon_test::Course;

namespace app_dto::course
{
    struct CourseDto
    {
        std::optional<int> id;
        std::string code;
        std::string name;
        int instructor_id;
        static CourseModel toModel(CourseDto data)
        {
            CourseModel course;
            if (data.id.has_value())
            {
                course.setId(data.id.value());
            }
            course.setCode(data.code);
            course.setName(data.name);
            course.setInstructorId(data.instructor_id);
            return course;
        }
        static CourseDto fromModel(CourseModel course)
        {
            CourseDto data;

            data.code = course.getValueOfCode();
            data.id = std::make_optional(course.getValueOfId());
            data.name = course.getValueOfName();
            data.instructor_id = course.getValueOfInstructorId();

            return data;
        }
    };

}

namespace drogon
{
    template <>
    inline app_dto::course::CourseDto fromRequest(const HttpRequest &req)
    {
        app_dto::course::CourseDto course;
        auto json = req.getJsonObject();

        if (json)
        {
            course.id = (*json)["id"].empty() ? std::nullopt : std::make_optional((*json)["id"].asInt());
            // course.id = (*json)["id"].asInt();
            course.code = (*json)["code"].asString();
            course.name = (*json)["name"].asString();
            course.instructor_id = (*json)["instructor_id"].asInt();
        }

        return course;
    }
}