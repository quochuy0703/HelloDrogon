#pragma once

#include <drogon/HttpController.h>

using namespace drogon;

namespace demo
{
  namespace v1
  {
    class User : public drogon::HttpController<User>
    {
    public:
      METHOD_LIST_BEGIN
      // use METHOD_ADD to add your custom processing function here;
      // METHOD_ADD(User::get, "/{2}/{1}", Get); // path is /demo/v1/User/{arg2}/{arg1}
      // METHOD_ADD(User::your_method_name, "/{1}/{2}/list", Get); // path is /demo/v1/User/{arg1}/{arg2}/list
      // ADD_METHOD_TO(User::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list

      METHOD_ADD(User::login, "/auth/login", Post);
      METHOD_ADD(User::csfrUserView, "/cross", Get);
      METHOD_ADD(User::getInfo, "/{1}/info?token={2}", Get, "AuthFilter");
      METHOD_ADD(User::loginAccount, "/auth/loginAccount", Post);
      METHOD_ADD(User::helloView, "/view", Get, "AuthViewFilter");
      METHOD_ADD(User::loginView, "/login", Post, Get);
      METHOD_ADD(User::upload, "/upload", Post);
      METHOD_ADD(User::listUserView, "/list", Get);
      METHOD_ADD(User::newUserView, "/new", Get);
      METHOD_ADD(User::insertUserView, "/insert", Post);
      METHOD_ADD(User::editUserView, "/edit/{1}", Get);
      METHOD_ADD(User::updateUserView, "/update", Post);
      METHOD_ADD(User::deleteUserView, "/delete/{1}", Get);
      METHOD_LIST_END
      // your declaration of processing function maybe like this:
      // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
      // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;
      void login(const HttpRequestPtr &req,
                 std::function<void(const HttpResponsePtr &)> &&callback);
      void loginView(const HttpRequestPtr &req,
                     std::function<void(const HttpResponsePtr &)> &&callback);
      void loginAccount(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback);
      drogon::AsyncTask upload(const HttpRequestPtr req,
                               std::function<void(const HttpResponsePtr &)> callback);
      void helloView(const HttpRequestPtr &req,
                     std::function<void(const HttpResponsePtr &)> &&callback);
      void listUserView(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback);
      void newUserView(const HttpRequestPtr &req,
                       std::function<void(const HttpResponsePtr &)> &&callback);
      void insertUserView(const HttpRequestPtr &req,
                          std::function<void(const HttpResponsePtr &)> &&callback);
      void editUserView(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback, std::string userId);
      void updateUserView(const HttpRequestPtr &req,
                          std::function<void(const HttpResponsePtr &)> &&callback);
      void deleteUserView(const HttpRequestPtr &req,
                          std::function<void(const HttpResponsePtr &)> &&callback, std::string userId);
      void csfrUserView(const HttpRequestPtr &req,
                        std::function<void(const HttpResponsePtr &)> &&callback);
      drogon::Task<> getInfo(const HttpRequestPtr req,
                             std::function<void(const HttpResponsePtr &)> callback,
                             std::string userId,
                             const std::string &token) const;
    };
  }
}
