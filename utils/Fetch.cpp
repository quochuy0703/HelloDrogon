#include "Fetch.hpp"
#include <drogon/HttpClient.h>

// Hàm kiểm tra xâu rỗng
bool IsNullOrEmpty(const std::string &str)
{
    return str.empty();
}

drogon::Task<std::string> sendRequest(const std::string &path, drogon::HttpMethod method, const std::string &endpoint, Json::Value body = Json::Value())
{
    std::string_view json = "";

    try
    {
        auto client = drogon::HttpClient::newHttpClient(endpoint);
        auto httpReq = drogon::HttpRequest::newHttpRequest();
        httpReq->setPath(path);
        httpReq->setMethod(method);

        if ((method == drogon::HttpMethod::Post || method == drogon::HttpMethod::Put) && body.size() != 0)
        {
            httpReq->setContentTypeCode(drogon::CT_APPLICATION_JSON);
            httpReq->setBody(body.toStyledString());
        }
        auto result = co_await client->sendRequestCoro(httpReq);
        json = result->getBody();
        LOG_INFO << std::string(json);
    }
    catch (drogon::HttpException ex)
    {
        LOG_INFO << ex.what();
    }

    co_return std::string(json);
};
std::string MapToQueryString(const std::map<std::string, std::string> &queries)
{
    std::string queryUri;
    for (const auto &[key, value] : queries)
    {
        if (!IsNullOrEmpty(value))
        {
            queryUri += key + "=" + value + "&";
        }
    }
    if (!queryUri.empty())
    {
        queryUri.pop_back(); // Loại bỏ ký tự '&' cuối cùng
    }
    return queryUri;
};

// Hàm chuyển đổi map thành chuỗi query string
drogon::Task<ApiResponse<Json::Value>> app_helpers::fetch_helper::Fetch::Get(const std::string &url, const std::map<std::string, std::string> &queries,
                                                                             const std::string &endpoint)
{
    Json::Value config = drogon::app().getCustomConfig();
    std::string fetchServer = config["REACT_APP_BASE_URL"].asString();
    std::string baseUrl = (endpoint.empty() ? fetchServer : endpoint);
    std::string path = "";

    if (!queries.empty())
    {
        std::string queryUri = MapToQueryString(queries);
        if (!queryUri.empty())
        {
            path = url + "?" + queryUri;
        }
    }

    // Builder builderRes = ApiResponse<T>::create();
    // builderRes.data(data).message(message).statusCode("200").success("ok").build()->toJson();

    auto result = co_await sendRequest(path, drogon::HttpMethod::Get, baseUrl);

    ApiResponse<Json::Value> api;
    auto res = api.create();
    co_return *(res.data(result).build());
}
drogon::Task<ApiResponse<Json::Value>> app_helpers::fetch_helper::Fetch::Post(const std::string &url, Json::Value body, const std::string &endpoint)
{
    Json::Value config = drogon::app().getCustomConfig();
    std::string fetchServer = config["REACT_APP_BASE_URL"].asString();
    std::string baseUrl = (endpoint.empty() ? fetchServer : endpoint);

    auto result = co_await sendRequest(url, drogon::HttpMethod::Post, baseUrl, body);

    ApiResponse<Json::Value> api;
    auto res = api.create();
    co_return *(res.data(result).build());
}
drogon::Task<ApiResponse<Json::Value>> app_helpers::fetch_helper::Fetch::Put(const std::string &url, Json::Value body, const std::string &endpoint)
{
    Json::Value config = drogon::app().getCustomConfig();
    std::string fetchServer = config["REACT_APP_BASE_URL"].asString();
    std::string baseUrl = (endpoint.empty() ? fetchServer : endpoint);

    auto result = co_await sendRequest(url, drogon::HttpMethod::Put, baseUrl, body);

    ApiResponse<Json::Value> api;
    auto res = api.create();
    co_return *(res.data(result).build());
};

drogon::Task<ApiResponse<Json::Value>> app_helpers::fetch_helper::Fetch::Delete(const std::string &url,
                                                                                const std::string &endpoint)
{
    Json::Value config = drogon::app().getCustomConfig();
    std::string fetchServer = config["REACT_APP_BASE_URL"].asString();
    std::string baseUrl = (endpoint.empty() ? fetchServer : endpoint);
    std::string path = url;

    auto result = co_await sendRequest(path, drogon::HttpMethod::Delete, baseUrl);

    ApiResponse<Json::Value> api;
    auto res = api.create();
    co_return *(res.data(result).build());
};
