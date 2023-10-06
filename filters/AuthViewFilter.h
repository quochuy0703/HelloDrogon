/**
 *
 *  AuthViewFilter.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class AuthViewFilter : public HttpFilter<AuthViewFilter>
{
  public:
    AuthViewFilter() {}
    void doFilter(const HttpRequestPtr &req,
                  FilterCallback &&fcb,
                  FilterChainCallback &&fccb) override;
};

