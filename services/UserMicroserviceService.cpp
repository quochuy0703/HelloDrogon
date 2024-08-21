#include "UserMicroserviceService.hpp"

namespace app_services::usermicroservice_service
{
    drogon::Task<std::vector<UserMicroserviceDto>> getAll()
    {

        std::vector<UserMicroserviceModel> users;
        try
        {
            users = co_await app_repositories::usermicroservice_repository::getAll();
        }
        catch (const std::exception &ex)
        {
            throw std::runtime_error(ex.what());
        }
        std::vector<UserMicroserviceDto> UserMicroserviceDtos;
        for (auto user : users)
        {
            UserMicroserviceDto temp = UserMicroserviceDto::fromModel(user);
            UserMicroserviceDtos.push_back(temp);
        }
        co_return UserMicroserviceDtos;
    }

    drogon::Task<UserMicroserviceDto> getById(std::string id)
    {
        UserMicroserviceModel user;
        try
        {
            user = co_await app_repositories::usermicroservice_repository::getById(id);
        }
        catch (const std::exception &ex)
        {
            throw std::runtime_error(ex.what());
        }
        co_return UserMicroserviceDto::fromModel(user);
    }

    drogon::Task<UserMicroserviceDto> create(UserMicroserviceDto data)
    {
        UserMicroserviceModel user = UserMicroserviceDto::toModel(data);
        user.setCreatedat(::trantor::Date().now());
        try
        {
            user = co_await app_repositories::usermicroservice_repository::create(user);
        }
        catch (const std::exception &ex)
        {
            throw std::runtime_error(ex.what());
        }

        co_return UserMicroserviceDto::fromModel(user);
    }

    drogon::Task<bool> update(UserMicroserviceDto data)
    {
        bool result;
        try
        {
            UserMicroserviceModel user = UserMicroserviceDto::toModel(data);

            result = co_await app_repositories::usermicroservice_repository::update(user);
        }
        catch (const std::exception &ex)
        {
            throw std::runtime_error(ex.what());
        }

        co_return result;
    }

    drogon::Task<bool> remove(std::string id)
    {

        bool result;
        try
        {
            result = co_await app_repositories::usermicroservice_repository::remove(id);
        }
        catch (const std::exception &ex)
        {
            throw std::runtime_error(ex.what());
        }
        co_return result;
    }

}
