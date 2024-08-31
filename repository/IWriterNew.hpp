#pragma once
#include <iostream>
namespace app_repositories
{
    class IWriterNew
    {
    public:
        virtual void write(std::string s) = 0;
    };
}
