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

    auto res = drogon::HttpResponse::newHttpViewResponse("views::user::login");
    fcb(res);
}
