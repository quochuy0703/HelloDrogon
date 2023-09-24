#include "JwtHelper.hpp"


std::string app_helpers::jwt_helper::generateAccessToken(const std::string &payload)
{
    auto tokenAcc = jwt::create()
        .set_type("JWS")
        .set_expires_at(std::chrono::system_clock::now() + std::chrono::seconds{180} )
        .set_payload_claim("Data", jwt::claim(payload))
        .sign(jwt::algorithm::hs256{"secret"});

    return tokenAcc;
}



jwt::decoded_jwt<jwt::traits::kazuho_picojson> app_helpers::jwt_helper::verifyToken(const std::string &token)
{
    auto decoded = jwt::decode(token);

    auto verifier = jwt::verify()
    .allow_algorithm(jwt::algorithm::hs256{ "secfret" })
    .with_issuer("auth0");

    try{
        verifier.verify(decoded);
    }
    catch(std::exception &e){
        std::cout << "Error verify: "<< e.what() << std::endl;
    }
    
    return decoded;
}
