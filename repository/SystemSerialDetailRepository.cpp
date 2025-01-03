#include "SystemSerialDetailRepository.hpp"
#include <iostream>

namespace app_repositories
{
    SystemSerialDetailRepository::SystemSerialDetailRepository(std::shared_ptr<drogon::orm::Transaction> txn)
        : tranPtr_(txn) { LOG_INFO << "Init PcPartRepository"; }

    SystemSerialDetailRepository::~SystemSerialDetailRepository()
    {
        // Destructor không cần làm gì thêm vì pqxx sẽ tự động quản lý
        LOG_INFO << "Destructor PcPartRepository";
    }

    drogon::Task<SystemSerialDetailRepository::Model> SystemSerialDetailRepository::getById(std::string code)
    {

        Model user;

        try
        {

            auto row = co_await tranPtr_->execSqlCoro("SELECT * FROM system_serial_detail where code = $1;", code);
            if (row.size() == 0)
            {
                throw drogon::orm::UnexpectedRows("O row found!");
            }
            user = Model(row.at(0));
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return user;
    }

    drogon::Task<std::vector<SystemSerialDetailRepository::Model>> SystemSerialDetailRepository::getAll()
    {

        std::vector<Model> users;
        try
        {
            auto rows = co_await tranPtr_->execSqlCoro("SELECT * FROM system_serial_detail;");
            for (auto row : rows)
            {
                users.push_back(Model(row));
            }
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return users;
    }

    drogon::Task<SystemSerialDetailRepository::Model> SystemSerialDetailRepository::add(const Model &Model)
    {

        try
        {
            bool sel = false;
            auto sql = Model.sqlForInserting(sel);
            LOG_INFO << sql;

            // auto row = co_await tranPtr_->execSqlCoro(sql, Model.getValueOfId(), Model.getValueOfCostcode(), Model.getValueOfPartcode(), Model.getValueOfPartname(), Model.getValueOfStatus(), Model.getValueOfCreateuser(), Model.getValueOfCreatedate(), Model.getValueOfUpdateuser(), Model.getValueOfUpdatedate());
            auto rows = co_await tranPtr_->execSqlCoro("SELECT * FROM system_serial_detail;");
            // LOG_INFO << row.affectedRows();
        }
        catch (const drogon::orm::DrogonDbException &ex)
        {
            LOG_ERROR << ex.base().what();
            throw std::runtime_error(ex.base().what());
        }

        co_return Model;
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
