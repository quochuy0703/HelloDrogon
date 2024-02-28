#include "EmailHelper.hpp"

using mailio::dialog_error;
using mailio::mail_address;
using mailio::message;
using mailio::smtp;
using mailio::smtp_error;
using mailio::smtps;
using std::cout;
using std::endl;

void app_helpers::email_helper::sendMail()
{
    try
    {
        // create mail message
        message msg;
        msg.from(mail_address("mailio library", "mailio@gmail.com"));          // set the correct sender name and address
        msg.add_recipient(mail_address("mailio library", "mailio@gmail.com")); // set the correct recipent name and address
        msg.subject("smtps simple message");
        msg.content("Hello, World!");

        // connect to server
        smtp conn("localhost", 1025);
        // modify username/password to use real credentials
        conn.authenticate("mailio@gmail.com", "mailiopass", smtp::auth_method_t::NONE);
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