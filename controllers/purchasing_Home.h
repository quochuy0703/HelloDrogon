#pragma once

#include <drogon/HttpController.h>
#include <iostream>

#include "../services/IPcPartService.hpp"

#include "../plugins/DIPlugin.h"
#include "../utils/ArrayHelper.hpp"
#include "../utils/CryptoHelper.hpp"
#include "../utils/sql.h"

using namespace drogon;
using namespace app_services;

namespace purchasing
{

  class Home : public drogon::HttpController<Home>
  {
  public:
    METHOD_LIST_BEGIN

    METHOD_ADD(Home::loginView, "", Get, Post);
    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;

    AsyncTask loginView(const HttpRequestPtr req,
                        std::function<void(const HttpResponsePtr &)> callback);
  };

}
