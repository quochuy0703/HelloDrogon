#include "purchasing_Home.h"

using namespace std;

using namespace purchasing;

// Add definition of your processing function here

AsyncTask Home::loginView(const HttpRequestPtr req,
                          std::function<void(const HttpResponsePtr &)> callback)
{
    try
    {
        auto *DIPluginPtr = app().getPlugin<drogon::plugin::DIPlugin>();

        if (req->getMethod() == drogon::HttpMethod::Get)
        {
            auto pcPartService = DIPluginPtr->get<IPcPartService>();

            LOG_INFO << "userService ptr count: " << pcPartService.use_count();

            auto listPart = co_await pcPartService->GetAll();

            auto data = HttpViewData();
            data["listPart"] = listPart;

            auto resp = HttpResponse::newHttpViewResponse("views::purchasing::Index", data);
            callback(resp);
            co_return;
        }
        std::string userId = req->getParameter("userId");
        std::string passwd = req->getParameter("password");
        LOG_INFO << "User: " << userId;
        auto hashPassword = app_helpers::crypto_helper::Base64Encode(app_helpers::crypto_helper::Base64Encode(passwd));
        }
    catch (exception &ex)
    {
        LOG_ERROR << ex.what();
        auto resp = HttpResponse::newHttpViewResponse("views::purchasing::Index");
        callback(resp);
    }
}
