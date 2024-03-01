#include "Fetch.hpp"

// Hàm kiểm tra xâu rỗng
bool IsNullOrEmpty(const std::string &str)
{
    return str.empty();
}

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
ApiResponse<Json::Value> app_helpers::fetch_helper::Fetch::Get(const std::string &url, const std::map<std::string, std::string> &queries,
                                                               const std::string &endpoint)
{
    Json::Value config = drogon::app().getCustomConfig();
    std::string fetchServer = config["REACT_APP_BASE_URL"].asString();
    std::string baseUrl = (endpoint.empty() ? fetchServer : endpoint) + url;

    if (!queries.empty())
    {
        std::string queryUri = MapToQueryString(queries);
        if (!queryUri.empty())
        {
            baseUrl += "?" + queryUri;
        }
    }

    // Tiếp tục xử lý với baseUrl...
    std::cout << baseUrl << std::endl; // In ra baseUrl làm ví dụ
    // Builder builderRes = ApiResponse<T>::create();
    // builderRes.data(data).message(message).statusCode("200").success("ok").build()->toJson();

    ApiResponse<Json::Value> api;
    auto res = api.create();
    return *(res.data(baseUrl).build());
};

ApiResponse<Json::Value> app_helpers::fetch_helper::Fetch::Delete(const std::string &url, const std::map<std::string, std::string> &queries,
                                                                  const std::string &endpoint)
{
    Json::Value config = drogon::app().getCustomConfig();
    std::string fetchServer = config["REACT_APP_BASE_URL"].asString();
    std::string baseUrl = (endpoint.empty() ? fetchServer : endpoint) + url;

    if (!queries.empty())
    {
        std::string queryUri = MapToQueryString(queries);
        if (!queryUri.empty())
        {
            baseUrl += "?" + queryUri;
        }
    }

    // Tiếp tục xử lý với baseUrl...
    std::cout << baseUrl << std::endl; // In ra baseUrl làm ví dụ
    // Builder builderRes = ApiResponse<T>::create();
    // builderRes.data(data).message(message).statusCode("200").success("ok").build()->toJson();

    ApiResponse<Json::Value> api;
    auto res = api.create();
    return *(res.data(baseUrl).build());
};
