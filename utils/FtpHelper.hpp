#include <iostream>
#include <drogon/drogon.h>
#include <ftplib.h>
#include <chrono>
#include "utils/ExecuteAwaiter.hpp"

using namespace drogon;

namespace app_helpers::ftp_helper
{
    class FtpHelper;
    struct AAwaiter : public CallbackAwaiter<std::shared_ptr<FtpHelper>>
    {

        void await_suspend(std::coroutine_handle<> handle)
        {

            std::cerr << "coroutine resume\n";
            auto func = std::async([&]()
                                   {
                try
                {
                    std::cout << "Sub: " << std::this_thread::get_id() << std::endl;
                    std::this_thread::sleep_for(std::chrono::seconds(60));
                    setValue(std::make_shared<FtpHelper>());
                }
                catch (std::exception &ex)
                {
                    throw std::runtime_error("Error FtpHelper: ");
                } });

            handle.resume();
        }
    };
    class FtpHelper
    {
    private:
        ftplib *client = nullptr;

    public:
        FtpHelper();
        ftplib *connect();
        void close();
        void uploadFile(const std::string &localFile, const std::string &remoteFile);
        app_helpers::execute_awaiter::ExecuteAwaiter uploadFileCoro(const std::string &localFile, const std::string &remoteFile);
        void uploadFileCallback(const std::string &localFile, const std::string &remoteFile, std::function<void(std::exception &ex)> = nullptr);
        void downloadFile(const std::string &pathFile, const std::string &outputFile);
    };

}
