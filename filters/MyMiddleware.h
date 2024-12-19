#pragma once

#include <drogon/HttpMiddleware.h>
using namespace drogon;

class MyMiddleware : public HttpMiddleware<MyMiddleware>
{
public:
    MyMiddleware() {}; // do not omit constructor

    void invoke(const HttpRequestPtr &req,
                MiddlewareNextCallback &&nextCb,
                MiddlewareCallback &&mcb) override;
};
