#include <iostream>

namespace app_helpers::crypto_helper
{
    std::string encrypt(const std::string &plain, const std::string &tokenHash);
    bool matches(const std::string &rawPassword, const std::string &hashPassword, const std::string &hashToken);
}