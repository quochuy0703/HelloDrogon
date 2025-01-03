#include "UnitOfWork.hpp"
#include <drogon/drogon.h>
#include <fruit/fruit.h>

namespace app_repositories::unitofwork
{
    // UnitOfWork::UnitOfWork(const drogon::orm::DbClientPtr &client)
    //     : dbClient_(client)
    // {
    //     LOG_INFO << "Init UnitOfWork";
    //     // auto db = drogon::app().getDbClient();
    // }

    UnitOfWork::UnitOfWork()

    {
        LOG_INFO << "Init UnitOfWork";
        dbClient_ = drogon::app().getDbClient("test");
        dbClientSmart_ = drogon::app().getDbClient("smart");

        std::cout << "dbClient: " << dbClient_.get() << " " << dbClientSmart_.get() << std::endl;
    }

    UnitOfWork::~UnitOfWork()
    {
        LOG_INFO << "Destructor UnitOfWork";
    }

    drogon::Task<> UnitOfWork::BeginTransaction()
    {
        if (!transaction_)
        {
            auto db = drogon::app().getDbClient();
            transaction_ = co_await dbClient_->newTransactionCoro();
            transactionSmart_ = co_await dbClientSmart_->newTransactionCoro();
            transaction_->setCommitCallback([](bool flag)
                                            { LOG_INFO << "Commit successfully!"; });
            transactionSmart_->setCommitCallback([](bool flag)
                                                 { LOG_INFO << "Commit Smart successfully!"; });

            usermicroserviceRepository_ = std::make_shared<UserMicroserviceRepository>(transaction_);
            pcPartRepository_ = std::make_shared<PcPartRepository>(transaction_);
        }
    }

    void UnitOfWork::Commit()
    {
        // if (transaction_)
        // {
        //     transaction_->commit();
        // }
        LOG_INFO << "transaction_ ptr count: " << transaction_.use_count() << " " << transactionSmart_.use_count();
        // transaction_.reset();
    }

    void UnitOfWork::Rollback()
    {
        if (transaction_)
        {
            transaction_->rollback();
            transactionSmart_->rollback();
        }
    }

    std::shared_ptr<UserMicroserviceRepository> UnitOfWork::UserMicroservices()
    {
        return this->usermicroserviceRepository_;
    }

    std::shared_ptr<PcPartRepository> UnitOfWork::PcPartRepositorys()
    {
        return this->pcPartRepository_;
    }
    std::shared_ptr<SystemSerialDetailRepository> UnitOfWork::SystemSerialDetailRepositories()
    {
        return this->systemSerialDetailRepository_;
    }
}
