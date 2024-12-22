#include "MyMiddleware.h"

// void MyMiddleware::invoke(const HttpRequestPtr &req,
//                           MiddlewareNextCallback &&nextCb,
//                           MiddlewareCallback &&mcb)
// {
//     LOG_INFO << "MyMiddleware";
//     const std::string &origin = req->getHeader("origin");
//     if (origin.find("localhost:3000") != std::string::npos)
//     {
//         // intercept directly
//         mcb(HttpResponse::newNotFoundResponse(req));
//         return;

//         // auto resp = HttpResponse::newNotFoundResponse(req);
//         // co_return resp;
//     }
//     // Do something before calling the next middleware
//     nextCb([mcb = std::move(mcb)](const HttpResponsePtr &resp)
//            {
//             // Do something after the next middleware returns
//             // resp->addHeader("Access-Control-Allow-Origin", origin);
//             // resp->addHeader("Access-Control-Allow-Credentials","true");
//             mcb(resp); });
//     // mcb(resp);
//     // return;

//     // auto resp = co_await next;
//     // co_return resp;
// }

Task<HttpResponsePtr> MyMiddleware::invoke(const HttpRequestPtr &req, MiddlewareNextAwaiter &&next)
{
    LOG_INFO << "MyMiddleware";
    const std::string &origin = req->getHeader("origin");
    if (origin.find("localhost:3000") != std::string::npos)
    {
        // intercept directly

        auto resp = HttpResponse::newNotFoundResponse(req);
        co_return resp;
    }
    // Do something before calling the next middleware

    auto resp = co_await next;
    co_return resp;
}
