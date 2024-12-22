#include "Fetch.hpp"
#include <drogon/HttpClient.h>

// Hàm kiểm tra xâu rỗng
bool IsNullOrEmpty(const std::string &str)
{
    return str.empty();
}

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

app_helpers::fetch_helper::Fetch::Fetch(const std::string &baseUrl)
{
    this->baseURL = baseUrl;
}

drogon::Task<ApiResponse<Json::Value>> app_helpers::fetch_helper::Fetch::Get(const std::string &url, const std::map<std::string, std::string> &queries)
{

    std::string path = "";

    if (!queries.empty())
    {
        std::string queryUri = MapToQueryString(queries);
        if (!queryUri.empty())
        {
            path = url + "?" + queryUri;
        }
    }

    auto result = co_await this->sendRequest(path, drogon::HttpMethod::Get);
    co_return result;
}
drogon::Task<ApiResponse<Json::Value>> app_helpers::fetch_helper::Fetch::Post(const std::string &url, Json::Value body)
{

    auto result = co_await this->sendRequest(url, drogon::HttpMethod::Post, body);
    co_return result;
}
drogon::Task<ApiResponse<Json::Value>> app_helpers::fetch_helper::Fetch::Put(const std::string &url, Json::Value body)
{

    auto result = co_await this->sendRequest(url, drogon::HttpMethod::Put, body);
    co_return result;
};

drogon::Task<ApiResponse<Json::Value>> app_helpers::fetch_helper::Fetch::Delete(const std::string &url, const std::map<std::string, std::string> &queries)
{
    std::string path = "";

    if (!queries.empty())
    {
        std::string queryUri = MapToQueryString(queries);
        if (!queryUri.empty())
        {
            path = url + "?" + queryUri;
        }
    }
    auto result = co_await this->sendRequest(path, drogon::HttpMethod::Delete);
    co_return result;
}
drogon::Task<ApiResponse<Json::Value>> app_helpers::fetch_helper::Fetch::sendRequest(const std::string &path, drogon::HttpMethod method, Json::Value body)
{
    std::string_view json = "";
    drogon::HttpResponsePtr result;
    ApiResponse<Json::Value> api;
    auto res = api.create();

    try
    {
        auto client = drogon::HttpClient::newHttpClient(this->baseURL);
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
