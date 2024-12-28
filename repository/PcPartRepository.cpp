#include "PcPartRepository.hpp"
#include <iostream>

namespace app_repositories
{
    PcPartRepository::PcPartRepository(std::shared_ptr<drogon::orm::Transaction> txn)
        : tranPtr_(txn) { LOG_INFO << "Init PcPartRepository"; }

    PcPartRepository::~PcPartRepository()
    {
        // Destructor không cần làm gì thêm vì pqxx sẽ tự động quản lý
        LOG_INFO << "Destructor PcPartRepository";
    }

    drogon::Task<Model> PcPartRepository::GetById(std::string id)
    {

        Model user;

        try
        {

            auto row = co_await tranPtr_->execSqlCoro("SELECT * FROM pc_part where userid = $1;", id);
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

    drogon::Task<std::vector<Model>> PcPartRepository::GetAll()
    {

        std::vector<Model> users;
        try
        {
            auto rows = co_await tranPtr_->execSqlCoro("SELECT * FROM pc_part;");
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

    drogon::Task<Model> PcPartRepository::Add(const Model &Model)
    {

        try
        {
            bool sel = false;
            auto sql = Model.sqlForInserting(sel);
            LOG_INFO << sql;

            auto row = co_await tranPtr_->execSqlCoro(sql, Model.getValueOfId(), Model.getValueOfCostcode(), Model.getValueOfPartcode(), Model.getValueOfPartname(), Model.getValueOfStatus(), Model.getValueOfCreateuser(), Model.getValueOfCreatedate(), Model.getValueOfUpdateuser(), Model.getValueOfUpdatedate());
            LOG_INFO << row.affectedRows();
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
