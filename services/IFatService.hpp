#pragma once

#include <drogon/drogon.h>

namespace app_services
{
    class IFatService
    {
    public:
        virtual drogon::Task<bool> getAll() = 0;
    };

}
