#include "EmailHelper.hpp"

using mailio::dialog_error;
using mailio::mail_address;
using mailio::message;
using mailio::smtp;
using mailio::smtp_error;
using mailio::smtps;
using std::cout;
using std::endl;

void app_helpers::email_helper::sendMailTest()
{
    try
    {
        // create mail message
        message msg;
        msg.from(mail_address("mailio library", "mailio@gmail.com"));            // set the correct sender name and address
        msg.add_recipient(mail_address("mailio library", "mailio@test.com"));    // set the correct recipent name and address
        msg.add_recipient(mail_address("mailio 2 library", "mailio2@test.com")); // set the correct recipent name and address
        msg.add_recipient(mail_address("", "mailio2@test.com"));
        msg.add_cc_recipient(mail_address("mailio cc library", "mailioaCC@test.com"));
        msg.add_bcc_recipient(mail_address("mailio bcc library", "mailioaBCC@test.com"));
        msg.subject("smtps simple message");
        msg.content("Hello, World!");
        // connect to server
        smtp conn("localhost", 1025);
        // modify username/password to use real credentials
        conn.authenticate("huymq@test.com", "12345678", smtp::auth_method_t::NONE);

        conn.submit(msg);
    }
    catch (smtp_error &exc)
    {
        cout << exc.what() << endl;
    }
    catch (dialog_error &exc)
    {
        cout << exc.what() << endl;
    }
}

void app_helpers::email_helper::sendMail(const std::string &from, const std::vector<std::string> &cc, const std::string &subject, const std::string &content, const std::vector<std::string> &to, const std::vector<std::string> &bcc, std::list<std::tuple<std::istream &, std::string, mailio::message::content_type_t>> atts)
{
    try
    {
        // create mail message
        message msg;
        msg.from(mail_address("", from));
        for (auto i : to)
        {
            msg.add_recipient(mail_address("", i));
        }
        for (auto i : cc)
        {
            msg.add_cc_recipient(mail_address("", i));
        }
        for (auto i : bcc)
        {
            msg.add_bcc_recipient(mail_address("", i));
        }

        msg.attach(atts);
        msg.subject(subject);
        msg.content(content);
        // connect to server
        smtp conn("localhost", 1025);
        // modify username/password to use real credentials
        conn.authenticate("huymq@test.com", "12345678", smtp::auth_method_t::NONE);

        conn.submit(msg);
    }
    catch (smtp_error &exc)
    {
        cout << exc.what() << endl;
    }
    catch (dialog_error &exc)
    {
        cout << exc.what() << endl;
    }
}
