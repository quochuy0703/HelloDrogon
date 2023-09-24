#include <iostream>

namespace app_helpers::crypto_helper{
    std::string encrypt(const std::string &password);
    bool matches(const std::string &rawPassword, const std::string &hashPassword);
}