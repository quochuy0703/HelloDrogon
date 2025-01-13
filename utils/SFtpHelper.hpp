#pragma once

#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <iostream>
#include <future>
#include <vector>

#ifndef O_RDONLY
#define O_RDONLY 0 // Định nghĩa nếu O_RDONLY không tồn tại
#endif

namespace app_helpers::sftp_helper
{
    class SFTPClient
    {
    public:
        SFTPClient(const std::string &host, const std::string &user, const std::string &password)
            : host_(host), user_(user), password_(password) {}
        ~SFTPClient();

        bool connect();
        std::future<void> downloadFileAsync(const std::string &remotePath, const std::string &localPath);
        void downloadFile(const std::string &remotePath, const std::string &localPath);

    private:
        std::string host_, user_, password_;
        ssh_session session_{nullptr};
        sftp_session sftp_{nullptr};
    };

}
