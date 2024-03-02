/**
 *
 *  AuthViewFilter.cc
 *
 */

#include "AuthViewFilter.h"
#include "../errors/ResourceNotFoundException.hpp"
#include "../utils/ApiResponse.hpp"
#include "../utils/JwtHelper.hpp"

using namespace drogon;

void AuthViewFilter::doFilter(const HttpRequestPtr &req,
                              FilterCallback &&fcb,
                              FilterChainCallback &&fccb)
{
    // Edit your logic here
    try
    {
        auto token = req->getCookie("token");
        LOG_INFO << req->getPath();
        // if (req->getPath().compare("/demo/v1/user/login") == 0)
        // {
        //     // Passed
        //     fccb();
        //     return;
        // };
        if (token == "")
            throw ResourceNotFoundException("Not token!");
        jwt::decoded_jwt<jwt::traits::kazuho_picojson> decoded = app_helpers::jwt_helper::verifyToken(token);

        // Passed
        if (req->getPath().compare("/demo/v1/user/login") == 0)
        {
            // redirect to dashboard
            auto res = drogon::HttpResponse::newRedirectionResponse("/demo/v1/user/view");
            fcb(res);

            return;
        };
        fccb();
        return;
    }
    catch (ResourceNotFoundException &ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    catch (std::runtime_error &ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    catch (std::exception &ex)
    {
        std::cout << "Error: " << ex.what() << std::endl;
    }
    // Check failed

    if (req->getPath().compare("/demo/v1/user/login") == 0)
    {
        // Passed
        // pass to login
        fccb();
        return;
    };
    auto res = drogon::HttpResponse::newRedirectionResponse("/demo/v1/user/login");
    fcb(res);
}
