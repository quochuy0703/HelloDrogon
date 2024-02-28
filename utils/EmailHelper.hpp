#include <iostream>
#include <mailio/message.hpp>
#include <mailio/smtp.hpp>

namespace app_helpers::email_helper
{
    void sendMailTest();
    void sendMail(const std::string &from, const std::vector<std::string> &to = {}, const std::string &subject = "", const std::string &content = "Hello World!", const std::vector<std::string> &cc = {}, const std::vector<std::string> &bcc = {}, std::list<std::tuple<std::istream &, std::string, mailio::message::content_type_t>> atts = {});
}
