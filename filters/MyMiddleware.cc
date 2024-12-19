#include "MyMiddleware.h"

void MyMiddleware::invoke(const HttpRequestPtr &req,
                          MiddlewareNextCallback &&nextCb,
                          MiddlewareCallback &&mcb)
{
    const std::string &origin = req->getHeader("origin");
    if (origin.find("localhost:3000") != std::string::npos)
    {
        // intercept directly
        mcb(HttpResponse::newNotFoundResponse(req));
        return;
    }
    // Do something before calling the next middleware
    nextCb([mcb = std::move(mcb)](const HttpResponsePtr &resp)
           {
            // Do something after the next middleware returns
            resp->addHeader("Access-Control-Allow-Origin", origin);
            resp->addHeader("Access-Control-Allow-Credentials","true");
            mcb(resp); });
}
