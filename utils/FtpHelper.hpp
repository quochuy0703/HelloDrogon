#include <ftplib.h>
#include <iostream>
#include <drogon/drogon.h>
#include <chrono>
#include "../utils/ExecuteAwaiter.hpp"

using namespace std::chrono;
using namespace drogon;

namespace app_helpers::ftp_helper
{
    struct AAwaiter;

    class FtpHelper
    {
    private:
        ftplib *client = nullptr;

    public:
        FtpHelper();
        ftplib *connect();
        void close();
        void uploadFile0(const std::string &localFile, const std::string &remoteFile);
        AAwaiter uploadFile(const std::string &localFile, const std::string &remoteFile);
        app_helpers::execute_awaiter::ExecuteAwaiter uploadFileCoro(const std::string &localFile, const std::string &remoteFile);
        void downloadFile(const std::string &pathFile, const std::string &outputFile);
    };
    struct AAwaiter : public CallbackAwaiter<std::shared_ptr<FtpHelper>>
    {
        AAwaiter(FtpHelper *client, std::string localFile, std::string remoteFile)
            : client_(client), localFile_(localFile), remoteFile_(remoteFile)
        {
        }
        void await_suspend(std::coroutine_handle<> handle)
        {
            setValue(std::make_shared<FtpHelper>());
            std::cerr << "coroutine resume\n";
            std::cout << "Sub Thread in Coro: " << std::this_thread::get_id() << std::endl;
            // std::this_thread::sleep_for(seconds(60));
            client_->uploadFile0(localFile_, remoteFile_);
            handle.resume();
        }

    private:
        FtpHelper *client_;
        std::string localFile_;
        std::string remoteFile_;
    };
}