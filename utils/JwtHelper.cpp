#include "JwtHelper.hpp"

std::string app_helpers::jwt_helper::generateAccessToken(const std::string &payload)
{
    const std::string secret = drogon::app().getCustomConfig()["jwt-secret"].asString();
    const std::string issuer = drogon::app().getCustomConfig()["jwt-issuer"].asString();
    const int duration = drogon::app().getCustomConfig()["jwt-accessTime"].asInt();
    auto tokenAcc = jwt::create()
                        .set_type("JWS")
                        .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{duration})
                        .set_payload_claim("Data", jwt::claim(payload))
                        .set_issuer(issuer)
                        .sign(jwt::algorithm::hs256{secret});

    return tokenAcc;
}

jwt::decoded_jwt<jwt::traits::kazuho_picojson> app_helpers::jwt_helper::verifyToken(const std::string &token)
{
    const std::string secret = drogon::app().getCustomConfig()["jwt-secret"].asString();
    const std::string issuer = drogon::app().getCustomConfig()["jwt-issuer"].asString();
    const int duration = drogon::app().getCustomConfig()["jwt-accessTime"].asInt();

    auto decoded = jwt::decode(token);

    auto verifier = jwt::verify()
                        .allow_algorithm(jwt::algorithm::hs256{secret})
                        .with_issuer(issuer);

    try
    {
        verifier.verify(decoded);
    }
    catch (std::exception &e)
    {
        std::cout << "Error verify: " << e.what() << std::endl;
        throw std::runtime_error("Error verify: " + std::string(e.what()));
    }

    return decoded;
}

app_helpers::execute_awaiter::ExecuteReturnAwaiter<std::string> app_helpers::jwt_helper::generateAccessTokenCoro(const std::string &payload)
{
    return app_helpers::execute_awaiter::executeIntensiveReturnFunction<std::string>([&]() -> std::string
                                                                                     { return app_helpers::jwt_helper::generateAccessToken(payload); });
}
