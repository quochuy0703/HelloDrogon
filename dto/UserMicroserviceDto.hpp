#pragma once

#include <iostream>
#include <optional>
#include <drogon/drogon.h>
#include "../models/UserMicroservice.h"

using UserMicroserviceModel = drogon_model::test::UserMicroservice;

namespace app_dto::userservice_dto
{
    struct UserMicroserviceDto
    {
        std::string userId;
        std::string email;
        std::string username;

        static UserMicroserviceModel toModel(UserMicroserviceDto data)
        {
            UserMicroserviceModel model;

            model.setUserid(data.userId);
            model.setEmail(data.email);
            model.setUsername(data.username);

            return model;
        }
        static UserMicroserviceDto fromModel(UserMicroserviceModel model)
        {
            UserMicroserviceDto data;
            data.userId = model.getValueOfUserid();
            data.username = model.getValueOfUsername();
            data.email = model.getValueOfEmail();

            return data;
        }

        static UserMicroserviceDto toDto(Json::Value root)
        {
            UserMicroserviceDto data;
            data.userId = root["UserId"].asString();
            data.username = root["UserName"].asString();
            data.email = root["Email"].asString();

            return data;
        }
    };

}

namespace drogon
{
    template <>
    inline app_dto::userservice_dto::UserMicroserviceDto fromRequest(const HttpRequest &req)
    {
        app_dto::userservice_dto::UserMicroserviceDto dto;
        auto json = req.getJsonObject();

        if (json)
        {
            dto.userId = (*json)["userId"].asString();
            // course.id = (*json)["id"].asInt();
            dto.username = (*json)["username"].asString();
            dto.email = (*json)["email"].asString();
        }

        return dto;
    }
}