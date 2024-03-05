#pragma once

#include <iostream>
#include <drogon/drogon.h>
#include "../models/UserLogin.h"

using UserLogin = drogon_model::test::UserLogin;

namespace app_dto::user
{
    struct UserDto
    {
        std::string id;
        std::string email;
        std::string plainPassword;
        std::string name;
        static UserLogin toUser(UserDto data)
        {
            UserLogin user;
            user.setId(std::stoi(data.id));
            user.setEmail(data.email);
            user.setName(data.name);
            return user;
        }
        static UserDto fromUser(UserLogin user)
        {
            UserDto data;
            data.email = *user.getEmail();
            data.id = std::to_string(*user.getId());
            data.name = *user.getName();
            return data;
        }
    };

}

namespace drogon
{
    template <>
    inline app_dto::user::UserDto fromRequest(const HttpRequest &req)
    {
        app_dto::user::UserDto user;

        user.id = req.getParameter("id");
        user.email = req.getParameter("email");
        user.name = req.getParameter("name");
        return user;
    }
}