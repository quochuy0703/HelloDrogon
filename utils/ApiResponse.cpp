// #include "ApiResponse.hpp"

// template <typename T>
// app_helpers::api_res_helper::ApiResponse<T>::Builder app_helpers::api_res_helper::ApiResponse<T>::create()
// {
//     return app_helpers::api_res_helper::ApiResponse<T>::Builder();
// }

// template <typename T>
// Json::Value app_helpers::api_res_helper::ApiResponse<T>::toJson()
// {
//     Json::Value ret;
//     ret["Data"] = this->data_;
//     ret["Message"] = this->message_;
//     ret["StatusCode"] = this->statusCode_;
//     ret["Success"] = this->success_;

//     return ret;
// }

// template <typename T>
// app_helpers::api_res_helper::ApiResponse<T>::Builder::Builder()
// {
//     this->api = new app_helpers::api_res_helper::ApiResponse<T>();
// }

// template <typename T>
// app_helpers::api_res_helper::ApiResponse<T>::Builder &app_helpers::api_res_helper::ApiResponse<T>::Builder::message(const std::string &message)
// {
//     this->api->message_ = message;
//     return *this;
// }

// template <typename T>
// app_helpers::api_res_helper::ApiResponse<T>::Builder &app_helpers::api_res_helper::ApiResponse<T>::Builder::data(const T &data)
// {
//     this->api->data_ = data;
//     return *this;
// }

// template <typename T>
// app_helpers::api_res_helper::ApiResponse<T>::Builder &app_helpers::api_res_helper::ApiResponse<T>::Builder::statusCode(const std::string &statusCode)
// {
//     this->api->statusCode_ = statusCode;
//     return *this;
// }

// template <typename T>
// app_helpers::api_res_helper::ApiResponse<T>::Builder &app_helpers::api_res_helper::ApiResponse<T>::Builder::success(const std::string &success)
// {
//     this->api->success_ = success;
//     return *this;
// }
// // template <typename T>
// // app_helpers::api_res_helper::ApiResponse *app_helpers::api_res_helper::ApiResponse<T>::Builder::build()
// // {
// //     return this->api;
// // }