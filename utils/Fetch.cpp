#include "Fetch.hpp"
#include <drogon/HttpClient.h>

// Hàm kiểm tra xâu rỗng
bool IsNullOrEmpty(const std::string &str)
{
    return str.empty();
}

drogon::Task<ApiResponse<Json::Value>> sendRequest(const std::string &path, drogon::HttpMethod method, const std::string &endpoint, Json::Value body = Json::Value())
{
    std::string_view json = "";
    drogon::HttpResponsePtr result;
    ApiResponse<Json::Value> api;
    auto res = api.create();

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
        result = co_await client->sendRequestCoro(httpReq);
        json = result->getBody();
        LOG_INFO << std::string(json);
        res.data(std::string(json)).message("Sucesss").success(true).statusCode(static_cast<int>(result->getStatusCode()));
    }
    catch (drogon::HttpException ex)
    {
        LOG_INFO << ex.what() << "," << ex.code();
        res.data(std::string(json)).message(ex.what()).success(false).statusCode(0);
    }

    co_return *(res.build());
};

// Hàm chuyển đổi map thành chuỗi query string
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

    auto result = co_await sendRequest(path, drogon::HttpMethod::Get, baseUrl);
    co_return result;
}
drogon::Task<ApiResponse<Json::Value>> app_helpers::fetch_helper::Fetch::Post(const std::string &url, Json::Value body, const std::string &endpoint)
{
    Json::Value config = drogon::app().getCustomConfig();
    std::string fetchServer = config["REACT_APP_BASE_URL"].asString();
    std::string baseUrl = (endpoint.empty() ? fetchServer : endpoint);

    auto result = co_await sendRequest(url, drogon::HttpMethod::Post, baseUrl, body);
    co_return result;
}
drogon::Task<ApiResponse<Json::Value>> app_helpers::fetch_helper::Fetch::Put(const std::string &url, Json::Value body, const std::string &endpoint)
{
    Json::Value config = drogon::app().getCustomConfig();
    std::string fetchServer = config["REACT_APP_BASE_URL"].asString();
    std::string baseUrl = (endpoint.empty() ? fetchServer : endpoint);

    auto result = co_await sendRequest(url, drogon::HttpMethod::Put, baseUrl, body);
    co_return result;
};

drogon::Task<ApiResponse<Json::Value>> app_helpers::fetch_helper::Fetch::Delete(const std::string &url,
                                                                                const std::string &endpoint)
{
    Json::Value config = drogon::app().getCustomConfig();
    std::string fetchServer = config["REACT_APP_BASE_URL"].asString();
    std::string baseUrl = (endpoint.empty() ? fetchServer : endpoint);
    std::string path = url;

    auto result = co_await sendRequest(path, drogon::HttpMethod::Delete, baseUrl);
    co_return result;
};
