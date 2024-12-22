#include <drogon/HttpMiddleware.h>
#include "../constants/app.hpp"
#include "../utils/ArrayHelper.hpp"
#include "../errors/ResourceNotFoundException.hpp"
#include "../utils/ApiResponse.hpp"
#include "../utils/JwtHelper.hpp"

using namespace drogon;

class VerifyAccessTokenMiddleware : public HttpCoroMiddleware<VerifyAccessTokenMiddleware>
// class MyMiddleware : public HttpMiddleware<MyMiddleware>
{
public:
    VerifyAccessTokenMiddleware() {}; // do not omit constructor

    // void invoke(const HttpRequestPtr &req,
    //             MiddlewareNextCallback &&nextCb,
    //             MiddlewareCallback &&mcb) override;
    Task<HttpResponsePtr> invoke(const HttpRequestPtr &req, MiddlewareNextAwaiter &&next) override;
};
