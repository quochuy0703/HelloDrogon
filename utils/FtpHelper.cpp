#include "FtpHelper.hpp"

app_helpers::ftp_helper::FtpHelper::FtpHelper()
{
    if (this->client == nullptr)
        this->client = new ftplib();
}

ftplib *app_helpers::ftp_helper::FtpHelper::connect()
{
    Json::Value config = drogon::app().getCustomConfig();
    std::string ftpServer = config["FTP_SERVER"].asString();
    std::string ftpPort = config["FTP_PORT"].asString();
    std::string ftpUser = config["FTP_USER"].asString();
    std::string ftpPwd = config["FPT_PWD"].asString();

    std::string host = ftpServer + ":" + ftpPort;

    try
    {
        bool isSuccess = this->client->Connect(host.c_str());
        if (!isSuccess)
        {
            throw runtime_error("Error");
        }
        isSuccess = this->client->Login(ftpUser.c_str(), ftpPwd.c_str());
        if (!isSuccess)
            throw std::runtime_error("Error");
    }
    catch (std::exception &ex)
    {
        const char *message = this->client->LastResponse();
        std::cout << "Error: " << message << std::endl;
        this->client->Quit();
    }

    return this->client;
}

void app_helpers::ftp_helper::FtpHelper::uploadFile(const std::string &localFile, const std::string &remoteFile)
{
    this->client->Put(localFile.c_str(), remoteFile.c_str(), ftplib::image);
}

void app_helpers::ftp_helper::FtpHelper::downloadFile(const std::string &pathFile, const std::string &outputFile)
{
    this->client->Get(outputFile.c_str(), pathFile.c_str(), ftplib::image);
}
