/**
 *
 *  AuthFilter.cc
 *
 */

#include "AuthFilter.h"
#include "../errors/ResourceNotFoundException.hpp"
#include "../utils/ApiResponse.hpp"
#include "../utils/JwtHelper.hpp"

using namespace drogon;

void AuthFilter::doFilter(const HttpRequestPtr &req,
                          FilterCallback &&fcb,
                          FilterChainCallback &&fccb)
{

    // Edit your logic here
    app_helpers::api_res_helper::ApiResponse<std::string>::Builder resBuilder = app_helpers::api_res_helper::ApiResponse<std::string>::create();
    std::string data = "", message = "";
    try
    {
        auto token = req->getHeader("x-access-token");
        if (token == "")
            throw ResourceNotFoundException("Not token!");
        jwt::decoded_jwt<jwt::traits::kazuho_picojson> decoded = app_helpers::jwt_helper::verifyToken(token);

        // Passed
        fccb();
        return;
    }
    catch (ResourceNotFoundException &ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
        message = ex.what();
    }
    catch (std::runtime_error &ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
        message = ex.what();
    }
    catch (std::exception &ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
        message = ex.what();
    }
    // Check failed

    Json::Value ret = resBuilder.data(data).message(message).statusCode(drogon::HttpStatusCode::k200OK).success(false).build()->toJson();
    auto res = drogon::HttpResponse::newHttpJsonResponse(ret);
    res->setStatusCode(k500InternalServerError);
    fcb(res);
}
