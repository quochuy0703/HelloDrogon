#include <ftplib.h>
#include <iostream>
#include <drogon/drogon.h>

namespace app_helpers::ftp_helper
{
    class FtpHelper
    {
    private:
        ftplib *client = nullptr;

    public:
        FtpHelper();
        ftplib *connect();
        void close();
        void uploadFile(const std::string &localFile, const std::string &remoteFile);
        void downloadFile(const std::string &pathFile, const std::string &outputFile);
    };
}