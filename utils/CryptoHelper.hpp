#include <iostream>
#include "utils/ExecuteAwaiter.hpp"

namespace app_helpers::crypto_helper
{
    std::string encrypt(const std::string &plain, const std::string &tokenHash);
    std::string decrypt(const std::string &hashData, const std::string &tokenHash);
    bool matches(const std::string &rawPassword, const std::string &hashPassword, const std::string &hashToken);
    app_helpers::execute_awaiter::ExecuteReturnAwaiter<bool> matchesCoro(const std::string &rawPassword, const std::string &hashPassword, const std::string &hashToken = "");
    std::string generateHMAC(const std::string &plain, const std::string &keyPlain = "");
    app_helpers::execute_awaiter::ExecuteReturnAwaiter<std::string> generateHMACCoro(const std::string &plain, const std::string &keyPlain = "");
    size_t generateBlockRandom(std::byte arr[], const size_t &length = 16);
}