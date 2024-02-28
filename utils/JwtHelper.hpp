#include <iostream>
#include <drogon/drogon.h>
#include <jwt-cpp/jwt.h>
#include "utils/ExecuteAwaiter.hpp"

namespace app_helpers::jwt_helper
{

    std::string generateAccessToken(const std::string &payload);
    app_helpers::execute_awaiter::ExecuteReturnAwaiter<std::string> generateAccessTokenCoro(const std::string &payload);
    jwt::decoded_jwt<jwt::traits::kazuho_picojson> verifyToken(const std::string &token);
}