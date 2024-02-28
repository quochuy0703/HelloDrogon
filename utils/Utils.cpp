#include "Utils.hpp"

#include <botan/bcrypt.h>
#include <botan/rng.h>
#include <botan/system_rng.h>

namespace app_helpers
{
    constexpr std::string_view WHITESPACE = " \n\r\t\f\v";

    std::string ltrim(const std::string &s)
    {
        size_t start = s.find_first_not_of(WHITESPACE);
        return (start == std::string::npos) ? "" : s.substr(start);
    }

    std::string &ltrim(std::string &s)
    {
        s.erase(0, s.find_first_not_of(WHITESPACE));
        return s;
    }

    std::string rtrim(const std::string &s)
    {
        size_t end = s.find_last_not_of(WHITESPACE);
        return (end == std::string::npos) ? "" : s.substr(0, end + 1);
    }

    std::string &rtrim(std::string &s)
    {
        s.erase(s.find_last_not_of(WHITESPACE) + 1);
        return s;
    }

    std::string trim(const std::string &s)
    {
        return rtrim(ltrim(s));
    }

    std::string &trim(std::string &s)
    {
        return rtrim(ltrim(s));
    }

    std::string bcrypt_hash(const std::string &txt)
    {
        return Botan::generate_bcrypt(txt, Botan::system_rng(), 10, 'y');
    }

    bool bcrypt_verify(const std::string &txt, const std::string &hash)
    {
        return Botan::check_bcrypt(txt, hash);
    }

    std::string json_encode(const Json::Value &json)
    {
        Json::StreamWriterBuilder builder;
        builder["commentStyle"] = "None";
        builder["indentation"] = "";
        return Json::writeString(builder, json);
    }

    Json::Value json_decode(const std::string &jsonStr)
    {
        Json::Value ret;

        Json::CharReaderBuilder builder;
        builder["collectComments"] = false;
        JSONCPP_STRING errs;
        std::stringstream ss(jsonStr);

        return Json::parseFromStream(builder, ss, &ret, &errs) ? ret : Json::nullValue;
    }

    std::vector<std::string> getRange(const std::string &startDate, const std::string &endDate, const std::string &type)
    {
        std::tm tmStartDate = {}, tmEndDate = {};
        std::istringstream ssStartDate(startDate), ssEndDate(endDate);
        ssStartDate >> std::get_time(&tmStartDate, "%Y-%m-%d");
        ssEndDate >> std::get_time(&tmEndDate, "%Y-%m-%d");
        std::time_t start = std::mktime(&tmStartDate);
        std::time_t end = std::mktime(&tmEndDate);
        std::vector<std::string> range;

        if (type == "days")
        {
            int diff = std::difftime(end, start) / (60 * 60 * 24);
            for (int i = 0; i <= diff; ++i)
            {
                std::time_t result = start + i * 60 * 60 * 24;
                std::tm *timeinfo = std::localtime(&result);
                std::stringstream ss;
                ss << std::put_time(timeinfo, "%Y%m%d");
                range.push_back(ss.str());
            }
        }
        else if (type == "months")
        {
            std::tm *timeinfo = std::localtime(&start);
            while (start <= end)
            {
                std::stringstream ss;
                ss << std::put_time(timeinfo, "%Y%m");
                range.push_back(ss.str());
                timeinfo->tm_mon++;
                start = std::mktime(timeinfo);
            }
        }
        else if (type == "years")
        {
            std::tm *timeinfo = std::localtime(&start);
            while (start <= end)
            {
                std::stringstream ss;
                ss << std::put_time(timeinfo, "%Y");
                range.push_back(ss.str());
                timeinfo->tm_year++;
                start = std::mktime(timeinfo);
            }
        }
        else
        {
            std::cerr << "Invalid type." << std::endl;
        }
        return range;
    }

}
