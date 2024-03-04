// #ifndef UTILS_HPP
// #define UTILS_HPP

#pragma once

#include <string>
#include <drogon/orm/Mapper.h>

namespace app_helpers
{
    std::string ltrim(const std::string &s);
    std::string &ltrim(std::string &s);
    std::string rtrim(const std::string &s);
    std::string &rtrim(std::string &s);
    std::string trim(const std::string &s);
    std::string &trim(std::string &s);

    std::string bcrypt_hash(const std::string &txt);
    bool bcrypt_verify(const std::string &txt, const std::string &hash);

    std::string json_encode(const Json::Value &json);
    Json::Value json_decode(const std::string &jsonStr);
    void repeatForFixedTime(int milliseconds);
    std::vector<std::string> getRange(const std::string &startDate, const std::string &endDate, const std::string &type = "days");

    template <typename T>
    std::string convertToString(T arg)
    {
        std::ostringstream oss;
        oss << arg;
        return oss.str();
    }

    // template <typename... Args>
    // std::vector<std::string> formatArgs(Args... args)
    // {
    //     std::vector<std::string> argList;
    //     (..., (argList.push_back(convertToString(args))));
    //     return argList;
    // }

    // Hàm template để thực hiện định dạng chuỗi tương tự hàm string.Format() trong C#
    template <typename... Args>
    std::string format(const std::string &fmt, Args... args)
    {
        std::ostringstream oss;
        size_t len = fmt.size();
        std::vector<std::string> argList;
        (..., (argList.push_back(convertToString(args))));

        size_t argIndex = 0;

        for (size_t i = 0; i < len; ++i)
        {
            if (fmt[i] == '{' && i + 1 < len && fmt[i + 1] >= '0' && fmt[i + 1] <= '9')
            {
                size_t j = i + 1;
                while (j < len && fmt[j] != '}')
                {
                    ++j;
                }
                if (j < len)
                {
                    int argNum = std::stoi(fmt.substr(i + 1, j - i - 1));
                    if (argNum >= 0 && argNum < sizeof...(Args))
                    {
                        oss << argList[argNum];
                    }
                    else
                    {
                        oss << "{" << argNum << "}";
                    }
                    i = j;
                }
            }
            else
            {
                oss << fmt[i];
            }
        }

        return oss.str();
    }

    // #include "Utils.cpp"

    // #endif
}
