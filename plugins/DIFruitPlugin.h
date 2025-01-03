#pragma once

#include <drogon/drogon.h>
#include <drogon/plugins/Plugin.h>
#include <boost/di.hpp>
#include "boost/di/extension/injector.hpp"
#include <iostream>
#include "../repository/IUnitOfWork.hpp"
#include "../repository/UnitOfWork.hpp"
#include "../repository/IRepository.hpp"
#include "../repository/UserMicroserviceRepository.hpp"
#include "../services/UserMicroserviceService.hpp"
#include "../services/IUserMicroserviceService.hpp"
#include <fruit/fruit.h>

#include "../services/IPcPartService.hpp"
#include "../services/PcPartService.hpp"

#include "../services/IFatService.hpp"
#include "../services/FatService.hpp"

#include "../services/ISystemSerialDetailService.hpp"
#include "../services/SystemSerialDetailService.hpp"

using namespace app_repositories;
using namespace app_services;

using fruit::Component;
using fruit::Injector;

namespace di = boost::di;

namespace drogon::plugin
{
    class DIFruitPlugin : public drogon::Plugin<DIFruitPlugin>
    {
    public:
        using InjectorType = Injector<app_services::IUserMicroserviceService, IPcPartService, IFatService, ISystemSerialDetailService>;
        DIFruitPlugin() = default;

        /// This method must be called by drogon to initialize and start the plugin.
        /// It must be implemented by the user.
        void initAndStart(const Json::Value &config) override;

        /// This method must be called by drogon to shutdown the plugin.
        /// It must be implemented by the user.
        void shutdown() override;

        template <typename T>
        T *get(const std ::string &sessionId)
        {
            auto it = sessions_.find(sessionId);

            if (it != sessions_.end())
            {
                return it->second->get<T *>();
            }
            else
            {
                throw std::runtime_error("Session not found: " + sessionId);
            }
        }

        // template <typename T>
        // T getUnique()
        // {
        //     return injector.create<T>();
        //     // return injector.create<T>();
        // }

        bool createSession(const std ::string &sessionId);
        bool endSession(const std ::string &sessionId);

    private:
        std::map<std::string, std::shared_ptr<InjectorType>> sessions_;
    };
}
