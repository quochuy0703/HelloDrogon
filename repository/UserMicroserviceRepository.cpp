#include "UserMicroserviceRepository.hpp"
#include <iostream>

namespace app_repositories
{
    UserMicroserviceRepository::UserMicroserviceRepository(std::shared_ptr<drogon::orm::Transaction> txn)
        : tranPtr_(txn) { LOG_INFO << "Init UserMicroserviceRepository"; }

    UserMicroserviceRepository::~UserMicroserviceRepository()
    {
        // Destructor không cần làm gì thêm vì pqxx sẽ tự động quản lý
        LOG_INFO << "Destructor UserMicroserviceRepository";
    }

    drogon::Task<UserMicroserviceModel> UserMicroserviceRepository::GetById(std::string id)
    {

        UserMicroserviceModel user;

        try
        {
            auto row = co_await tranPtr_->execSqlCoro("SELECT * FROM users_microservice where userid = $1;", id);
            if (row.size() == 0)
            {
                throw drogon::orm::UnexpectedRows("O row found!");
            }
            user = UserMicroserviceModel(row.at(0));
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return user;
    }

    drogon::Task<std::vector<UserMicroserviceModel>> UserMicroserviceRepository::GetAll()
    {

        std::vector<UserMicroserviceModel> users;
        try
        {
            auto rows = co_await tranPtr_->execSqlCoro("SELECT * FROM user_microservice;");
            for (auto row : rows)
            {
                users.push_back(UserMicroserviceModel(row));
            }
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return users;
    }

    drogon::Task<UserMicroserviceModel> UserMicroserviceRepository::Add(const UserMicroserviceModel &model)
    {

        try
        {
            bool sel = false;
            auto sql = model.sqlForInserting(sel);
            LOG_INFO << sql;

            auto row = co_await tranPtr_->execSqlCoro(sql, model.getValueOfUserid(), model.getValueOfUsername(), model.getValueOfEmail(), model.getValueOfCreatedat());
            LOG_INFO << row.affectedRows();
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return model;
    }

    // void UserMicroserviceRepository::Remove(int id)
    // {
    //     try
    //     {
    //         txn->exec0("DELETE FROM users WHERE id = " + txn->quote(id));
    //     }
    //     catch (const std::exception &e)
    //     {
    //         std::cerr << "Error removing user: " << e.what() << std::endl;
    //     }
    // }
}
