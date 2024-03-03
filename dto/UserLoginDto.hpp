#include <iostream>
#include <drogon/drogon.h>

namespace app_dto::user_login
{
    struct UserLoginDto
    {
        std::string userId;
        std::string password;
    };

}

namespace drogon
{
    template <>
    inline app_dto::user_login::UserLoginDto fromRequest(const HttpRequest &req)
    {
        auto json = req.getJsonObject();
        app_dto::user_login::UserLoginDto user;
        if (json)
        {
            user.userId = (*json)["userId"].asString();
            user.password = (*json)["password"].asString();
        }
        return user;
    }
}