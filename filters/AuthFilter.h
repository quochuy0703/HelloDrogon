/**
 *
 *  AuthFilter.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class AuthFilter : public HttpFilter<AuthFilter>
{
  public:
    AuthFilter() {}
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;
};



// /**
//  *
//  *  JwtFilter.h
//  *
//  */

// #pragma once

// #include <drogon/HttpFilter.h>
// using namespace drogon;

// namespace api::v1::filters
// {

//   class JwtFilter : public drogon::HttpCoroFilter<JwtFilter>
//   {
//   public:
//     JwtFilter() = default;

//     virtual Task<HttpResponsePtr> doFilter(const HttpRequestPtr &req) override;
//   };

// }


// /**
//  *
//  *  JwtFilter.cc
//  *
//  */

// #include "JwtFilter.h"
// #include "jwt/JWT.h"

// using namespace drogon;
// using namespace Utils::jwt;

// namespace api::v1::filters
// {

// 	Task<HttpResponsePtr> JwtFilter::doFilter(const HttpRequestPtr &req)
// 	{
// 		// Skip the verification on method Options
// 		if (req->getMethod() == HttpMethod::Options)
// 			co_return {};

// 		auto headers = req->getHeaders();
// 		auto token = headers["token"], userid = headers["userid"];

// 		if (token.empty() || userid.empty())
// 		{
// 			Json::Value resultJson;
// 			resultJson["code"] = k401Unauthorized;
// 			resultJson["msg"] = "Authentication Error.";

// 			auto resp = HttpResponse::newHttpJsonResponse(resultJson);
// 			resp->setStatusCode(k401Unauthorized);

// 			co_return resp;
// 		}

// 		try
// 		{
// 			auto redisClientPtr = drogon::app().getRedisClient();
// 			auto auth = drogon::app().getCustomConfig()["redis"]["auth_key"].asString() + "_" + userid;
// 			// auto bcryptStr = redisClientPtr->execCommandSync<std::string>(
// 			// 		[](const nosql::RedisResult &r)
// 			// 		{
// 			// 			if (r.type() == nosql::RedisResultType::kNil)
// 			// 				return std::string{};
// 			// 			return r.asString();
// 			// 		},
// 			// 		"hget %s %s", auth.data(), token.data());
// 			auto result = co_await redisClientPtr->execCommandCoro("hget %s %s", auth.data(), token.data());
// 			if (result.isNil())
// 			{
// 				Json::Value resultJson;
// 				resultJson["code"] = k401Unauthorized;
// 				resultJson["msg"] = "Token is invalid.";
// 				auto resp = HttpResponse::newHttpJsonResponse(resultJson);
// 				resp->setStatusCode(k401Unauthorized);
// 				co_return resp;
// 			}

// 			auto jwtAttributes = JWT::decodeToken(result.asString());

// 			if ((&jwtAttributes) == nullptr)
// 			{
// 				Json::Value resultJson;
// 				resultJson["code"] = k401Unauthorized;
// 				resultJson["msg"] = "Token is invalid.";
// 				auto resp = HttpResponse::newHttpJsonResponse(resultJson);
// 				resp->setStatusCode(k401Unauthorized);
// 				co_return resp;
// 			}

// 			auto member = jwtAttributes.member;
// 			if (!member)
// 			{
// 				// The validity period is automatically extended by one hour.
// 				// redisClientPtr->execCommandSync<int>(
// 				// 		[](const nosql::RedisResult &r)
// 				// 		{
// 				// 			return r.asInteger();
// 				// 		},
// 				// 		"expire %s %i", auth.data(), 3600);
// 				co_await redisClientPtr->execCommandCoro("expire %s %i", auth.data(), 3600);
// 			}

// 			// Save the claims on attributes, for on next endpoint to be accessible
// 			req->getAttributes()->insert("jwt_userid", jwtAttributes.userid);

// 			co_return {};
// 		}
// 		catch (const std::exception &err)
// 		{
// 			LOG_ERROR << "JwtFilter error: " << err.what();
// 			Json::Value resultJson;
// 			resultJson["code"] = k401Unauthorized;
// 			resultJson["msg"] = err.what();
// 			auto resp = HttpResponse::newHttpJsonResponse(resultJson);
// 			resp->setStatusCode(k401Unauthorized);
// 			co_return resp;
// 		}
// 	}

// }
