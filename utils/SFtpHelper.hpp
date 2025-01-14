#pragma once

#include <libssh/libssh.h>
#include <libssh/sftp.h>
#include <iostream>
#include <future>
#include <vector>
#include <fstream>
#include "utils/ExecuteAwaiter.hpp"

#ifndef O_RDONLY
#define O_RDONLY 0x0000
#define O_WRONLY 0x0001
#define O_RDWR 0x0002
#define O_APPEND 0x0008
#define O_CREAT 0x0100
#define O_TRUNC 0x0200
#define O_EXCL 0x0400
#define O_TEXT 0x4000
#define O_BINARY 0x8000
#define O_WTEXT 0x10000
#define O_U16TEXT 0x20000
#define O_U8TEXT 0x40000
#define O_ACCMODE (O_RDONLY | O_WRONLY | O_RDWR)
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
        app_helpers::execute_awaiter::ExecuteAwaiter downloadFileCoro(const std::string &remotePath, const std::string &localPath);
        app_helpers::execute_awaiter::ExecuteAwaiter uploadFileCoro(const std::string &remotePath, const std::string &localPath);
        void downloadFile(const std::string &remotePath, const std::string &localPath);
        void uploadFile(const std::string &remotePath, const std::string &localPath);

    private:
        std::string host_,
            user_,
            password_;
        ssh_session session_{nullptr};
        sftp_session sftp_{nullptr};
    };

}
