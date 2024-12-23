#include "VerifyAccessTokenMiddleware.h"

Task<HttpResponsePtr> VerifyAccessTokenMiddleware::invoke(const HttpRequestPtr &req, MiddlewareNextAwaiter &&next)
{
    LOG_INFO << "VerifyAccessTokenMiddleware";

    app_helpers::api_res_helper::ApiResponse<std::string>::Builder resBuilder = app_helpers::api_res_helper::ApiResponse<std::string>::create();
    std::string data = "", message = "";
    HttpResponsePtr resp;

    std::string uid = "";

    app_helpers::array_helper::ArrayVector<std::string> routeExcept(constants::EXCEPT_ROUTE);
    auto idx = routeExcept.findIndex([req](auto item)
                                     { return (item.find(req->path()) != std::string::npos); });

    // int idx = -1;
    try
    {
        if (idx >= 0)
        {
            resp = co_await next;
        }
        else
        {
            auto token = req->getHeader("authorization");
            if (token == "")
            {
                throw ResourceNotFoundException("Not token!");
            }
            jwt::decoded_jwt<jwt::traits::nlohmann_json> decoded = app_helpers::jwt_helper::verifyToken(token);
            auto jsonData = std::string(decoded.get_payload_claim("Data").as_string().c_str());
            jwt::traits::nlohmann_json::json root;
            auto json2 = jwt::traits::nlohmann_json::parse(root, jsonData);

            uid = (root.contains("uid") && root["uid"].is_string()) ? root["uid"].get<std::string>() : root["username"].get<std::string>();

            if (uid.empty())
            {
                message = "Invalid";
                Json::Value ret = resBuilder.data(data).message(message).statusCode(drogon::HttpStatusCode::k200OK).success(false).build()->toJson();
                resp = drogon::HttpResponse::newHttpJsonResponse(ret);
                resp->setStatusCode(k200OK);
                co_return resp;
            }

            req->setParameter("username", uid);

            resp = co_await next;
        }
    }
    catch (ResourceNotFoundException &ex)
    {
        LOG_ERROR << "Error: " << ex.what();
        // message = ex.what();
        message = "Unauthorized";
        Json::Value ret = resBuilder.data(data).message(message).statusCode(drogon::HttpStatusCode::k403Forbidden).success(false).build()->toJson();
        resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k200OK);
    }
    catch (std::runtime_error &ex)
    {
        LOG_ERROR << "Error: " << ex.what();
        // message = ex.what();
        message = "Unauthorized";
        Json::Value ret = resBuilder.data(data).message(message).statusCode(drogon::HttpStatusCode::k403Forbidden).success(false).build()->toJson();
        resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k200OK);
    }
    catch (std::exception &ex)
    {
        LOG_ERROR << "Error: " << ex.what();
        // message = ex.what();' message = "Unauthorized";
        Json::Value ret = resBuilder.data(data).message(message).statusCode(drogon::HttpStatusCode::k403Forbidden).success(false).build()->toJson();
        resp = drogon::HttpResponse::newHttpJsonResponse(ret);
        resp->setStatusCode(k200OK);
    }

    co_return resp;
}
