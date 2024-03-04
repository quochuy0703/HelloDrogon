#include <iostream>
#include <drogon/drogon.h>
#include "../models/User.h"

using UserLogin = drogon_model::drogon_test::User;

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

// namespace drogon
// {
//     template <>
//     inline app_dto::user::UserDto fromRequest(const HttpRequest &req)
//     {
//         auto json = req.getJsonObject();
//         app_dto::user_login::UserLoginDto user;
//         if (json)
//         {
//             user.userId = (*json)["userId"].asString();
//             user.password = (*json)["password"].asString();
//         }
//         return user;
//     }
// }