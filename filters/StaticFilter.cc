/**
 *
 *  StaticFilter.cc
 *
 */

#include "StaticFilter.h"

using namespace drogon;

void StaticFilter::doFilter(const HttpRequestPtr &req,
                            FilterCallback &&fcb,
                            FilterChainCallback &&fccb)
{
    // Edit your logic here
    if (1)
    {
        LOG_INFO << "Path: " << req->path();
        // Passed
        fccb();
        return;
    }
    // Check failed
    auto res = drogon::HttpResponse::newHttpResponse();
    res->setStatusCode(k500InternalServerError);
    fcb(res);
}
