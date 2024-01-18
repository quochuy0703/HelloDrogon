/**
 *
 *  CSRFFilter.cc
 *
 */

#include "CSRFFilter.h"
#include "../utils/JwtHelper.hpp"

using namespace drogon;

void CSRFFilter::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb)
{
    //Edit your logic here
    if (1)
    {
        auto att = req->getAttributes();
        auto decoded =  att->get<std::string>("decoded");

        std::string csrf_token = app_helpers::jwt_helper::generateTokenWithDuration(120, decoded);
        att->insert("csrf_token", csrf_token);

        //Passed
        fccb();
        return;
    }
    //Check failed
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(k500InternalServerError);
    fcb(res);
}
