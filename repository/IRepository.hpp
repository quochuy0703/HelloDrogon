#pragma once
#include <vector>
#include <drogon/drogon.h>

namespace app_repositories
{
    template <typename T>
    class IRepository
    {
    public:
        virtual ~IRepository() = default;
        virtual drogon::Task<T> GetById(std::string id) = 0;
        virtual drogon::Task<std::vector<T>> GetAll() = 0;
        virtual drogon::Task<T> Add(const T &entity) = 0;
        // virtual void Remove(int id) = 0;
    };

}
