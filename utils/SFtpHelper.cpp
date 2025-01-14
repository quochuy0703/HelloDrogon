#include "SFtpHelper.hpp"

bool app_helpers::sftp_helper::SFTPClient::connect()
{
    session_ = ssh_new();
    if (!session_)
        return false;

    ssh_options_set(session_, SSH_OPTIONS_HOST, host_.c_str());
    // Thiết lập thông tin kết nối
    int port = 2121;
    ssh_options_set(session_, SSH_OPTIONS_PORT, &port);
    int verbosity = SSH_LOG_PROTOCOL;
    ssh_options_set(session_, SSH_OPTIONS_LOG_VERBOSITY, &verbosity);
    if (ssh_connect(session_) != SSH_OK)
    {
        std::cerr << "SSH Connection failed: " << ssh_get_error(session_) << std::endl;
        return false;
    }

    if (ssh_userauth_password(session_, user_.c_str(), password_.c_str()) != SSH_AUTH_SUCCESS)
    {
        std::cerr << "Authentication failed: " << ssh_get_error(session_) << std::endl;
        return false;
    }
    std::cout << "Connected to " << host_ << std::endl;
    return true;
}

std::future<void> app_helpers::sftp_helper::SFTPClient::downloadFileAsync(const std::string &remotePath, const std::string &localPath)
{
    return std::async(std::launch::async, &SFTPClient::downloadFile, this, remotePath, localPath);
}

app_helpers::execute_awaiter::ExecuteAwaiter app_helpers::sftp_helper::SFTPClient::downloadFileCoro(const std::string &remotePath, const std::string &localPath)
{
    std::cout << "Main: " << std::this_thread::get_id() << std::endl;
    return app_helpers::execute_awaiter::executeIntensiveFunction([&]()
                                                                  { app_helpers::sftp_helper::SFTPClient::downloadFileCoro(remotePath, localPath); });
}

void app_helpers::sftp_helper::SFTPClient::downloadFile(const std::string &remotePath, const std::string &localPath)
{
    sftp_ = sftp_new(session_);
    if (!sftp_)
    {
        std::cerr << "SFTP session failed: " << ssh_get_error(session_) << std::endl;
        return;
    }
    if (sftp_init(sftp_) != SSH_OK)
    {
        std::cerr << "SFTP initialization failed: " << sftp_get_error(sftp_) << std::endl;
        return;
    }

    sftp_file file = sftp_open(sftp_, remotePath.c_str(), O_RDONLY, 0);
    if (!file)
    {
        std::cerr << "Failed to open file: " << remotePath << std::endl;
        return;
    }

    FILE *localFile = fopen(localPath.c_str(), "wb");
    if (!localFile)
    {
        std::cerr << "Failed to open local file: " << localPath << std::endl;
        sftp_close(file);
        return;
    }

    char buffer[8192];
    int bytesRead;
    while ((bytesRead = sftp_read(file, buffer, sizeof(buffer))) > 0)
    {
        fwrite(buffer, 1, bytesRead, localFile);
    }

    std::cout << "File downloaded: " << localPath << std::endl;
    fclose(localFile);
    sftp_close(file);
}

app_helpers::execute_awaiter::ExecuteAwaiter app_helpers::sftp_helper::SFTPClient::uploadFileCoro(const std::string &remotePath, const std::string &localPath)
{
    std::cout << "Main: " << std::this_thread::get_id() << std::endl;
    return app_helpers::execute_awaiter::executeIntensiveFunction([&]()
                                                                  { app_helpers::sftp_helper::SFTPClient::uploadFileCoro(remotePath, localPath); });
}

void app_helpers::sftp_helper::SFTPClient::uploadFile(const std::string &remotePath, const std::string &localPath)
{
    sftp_ = sftp_new(session_);
    if (!sftp_)
    {
        std::cerr << "SFTP session failed: " << ssh_get_error(session_) << std::endl;
        return;
    }
    if (sftp_init(sftp_) != SSH_OK)
    {
        std::cerr << "SFTP initialization failed: " << sftp_get_error(sftp_) << std::endl;
        return;
    }

    sftp_file file = sftp_open(sftp_, remotePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0);
    if (!file)
    {
        std::cerr << "Failed to open file: " << remotePath << std::endl;
        return;
    }

    std::ifstream *localFile = new std::ifstream(localPath.c_str(), std::ios::binary);
    if (!localFile)
    {
        std::cerr << "Failed to open local file: " << localPath << std::endl;
        sftp_close(file);
        return;
    }

    // Read from local file and write to remote file
    char buffer[4096];
    while (localFile->read(buffer, sizeof(buffer)) || localFile->gcount() > 0)
    {
        ssize_t bytesWritten = sftp_write(file, buffer, localFile->gcount());
        if (bytesWritten < 0)
        {
            std::cerr << "Error writing to remote file: " << ssh_get_error(session_) << "\n";
            localFile->close();
            sftp_close(file);
            sftp_free(sftp_);
            ssh_disconnect(session_);
            ssh_free(session_);
            return;
        }
    }

    std::cout << "File uploaded: " << localPath << std::endl;
    localFile->close();
    sftp_close(file);
}

app_helpers::sftp_helper::SFTPClient::~SFTPClient()
{
    if (sftp_)
        sftp_free(sftp_);
    if (session_)
        ssh_disconnect(session_);
    ssh_free(session_);
}
