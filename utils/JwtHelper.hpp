#include <iostream>
#include <drogon/drogon.h>
#include <jwt-cpp/jwt.h>
#include <jwt-cpp/traits/nlohmann-json/traits.h>
#include "utils/ExecuteAwaiter.hpp"

namespace app_helpers::jwt_helper
{

    std::string generateAccessToken(const std::string &payload);
    app_helpers::execute_awaiter::ExecuteReturnAwaiter<std::string> generateAccessTokenCoro(const std::string &payload);
    jwt::decoded_jwt<jwt::traits::nlohmann_json> verifyToken(const std::string &token);
}