#include "FtpHelper.hpp"
#include "CryptoHelper.hpp"

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
    std::string ftpPwdHash = config["FTP_PWD"].asString();
    std::string ftpPwd = app_helpers::crypto_helper::decrypt(ftpPwdHash, "");

    std::string host = ftpServer + ":" + ftpPort;

    try
    {
        bool isSuccess = this->client->Connect(host.c_str());
        if (!isSuccess)
        {
            throw runtime_error("Error FtpHelper: Can't connect Ftp server!");
        }
        isSuccess = this->client->Login(ftpUser.c_str(), ftpPwd.c_str());
        if (!isSuccess)
            throw std::runtime_error("Error FtpHelper: Login Ftp server fail!");
    }
    catch (std::exception &ex)
    {
        const char *message = this->client->LastResponse();
        std::cout << "Error FtpHelper: " << message << std::endl;
        throw std::runtime_error("Error FtpHelper: " + std::string(message));
    }

    return this->client;
}

void app_helpers::ftp_helper::FtpHelper::close()
{
    if (this->client != nullptr)
        this->client->Quit();
}

void app_helpers::ftp_helper::FtpHelper::uploadFile(const std::string &localFile, const std::string &remoteFile)
{
    std::cout << "Main upload: " << std::this_thread::get_id() << std::endl;
    try
    {
        bool isSuccess = this->client->Put(localFile.data(), remoteFile.data(), ftplib::image);
        std::this_thread::sleep_for(std::chrono::seconds(60));
        if (!isSuccess)
            throw std::runtime_error("Error FtpHelper: Can't upload to Ftp server!");
    }
    catch (std::exception &ex)
    {
        const char *message = this->client->LastResponse();
        std::cout << "Error FtpHelper: " << message << std::endl;
        this->client->Quit();
        throw std::runtime_error("Error FtpHelper: " + std::string(message));
    }
}

app_helpers::execute_awaiter::ExecuteAwaiter app_helpers::ftp_helper::FtpHelper::uploadFileCoro(const std::string &localFile, const std::string &remoteFile)
{
    std::cout << "Main: " << std::this_thread::get_id() << std::endl;
    return app_helpers::execute_awaiter::executeIntensiveFunction([&]()
                                                                  { app_helpers::ftp_helper::FtpHelper::uploadFile(localFile, remoteFile); });
}

void app_helpers::ftp_helper::FtpHelper::uploadFileCallback(const std::string &localFile, const std::string &remoteFile, std::function<void(std::exception &ex)> callback)
{
    try
    {
        std::cout << "Sub: " << std::this_thread::get_id() << std::endl;
        bool isSuccess = this->client->Put(localFile.data(), remoteFile.data(), ftplib::image);
        std::this_thread::sleep_for(std::chrono::seconds(60));
        if (!isSuccess)
            throw std::runtime_error("Error FtpHelper: Can't upload to Ftp server!");
    }
    catch (std::exception &ex)
    {
        const char *message = this->client->LastResponse();
        std::cout << "Error FtpHelper: " << message << std::endl;
        this->client->Quit();
        if (callback == nullptr)
            throw std::runtime_error("Error FtpHelper: " + std::string(message));
        callback(ex);
    }
}

void app_helpers::ftp_helper::FtpHelper::downloadFile(const std::string &pathFile, const std::string &outputFile)
{
    try
    {
        bool isSuccess = this->client->Get(outputFile.c_str(), pathFile.c_str(), ftplib::image);
        if (!isSuccess)
            throw std::runtime_error("Error FtpHelper: Can't download from Ftp server!");
    }
    catch (std::exception &ex)
    {
        const char *message = this->client->LastResponse();
        std::cout << "Error FtpHelper: " << message << std::endl;
        this->client->Quit();
        throw std::runtime_error("Error FtpHelper: " + std::string(message));
    }
}
