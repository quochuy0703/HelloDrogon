#include <iostream>
#include <drogon/drogon.h>
#include <jwt-cpp/jwt.h>

namespace app_helpers::jwt_helper{

    std::string generateTokenWithDuration(const int &duration, const std::string &payload);
    std::string generateAccessToken(const std::string &payload);
    jwt::decoded_jwt<jwt::traits::kazuho_picojson> verifyToken(const std::string &token);
}