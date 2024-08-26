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

using namespace app_repositories;
using namespace app_services;

namespace di = boost::di;

namespace drogon::plugin
{
    class DIPlugin : public drogon::Plugin<DIPlugin>
    {
    public:
        DIPlugin() = default;

        /// This method must be called by drogon to initialize and start the plugin.
        /// It must be implemented by the user.
        void initAndStart(const Json::Value &config) override;

        /// This method must be called by drogon to shutdown the plugin.
        /// It must be implemented by the user.
        void shutdown() override;

        template <typename T>
        std::shared_ptr<T> get()
        {
            return injector.create<std::shared_ptr<T>>();
            // return injector.create<T>();
        }

        template <typename T>
        T getUnique()
        {
            return injector.create<T>();
            // return injector.create<T>();
        }

        // inline auto MakeAppInjector()
        // {
        //     auto dbClient = drogon::app().getDbClient();

        //     return di::make_injector(
        //         di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>(),  // UnitOfWork là singleton
        //         di::bind<drogon::orm::DbClient>.to(dbClient.get()),              // Inject DbClient vào UnitOfWork
        //         di::bind<IUserMicroserviceService>.to<UserMicroserviceService>() // UserService nhận UnitOfWork
        //     );
        // }

        // using AppInjectorType = decltype(MakeAppInjector());

        // extern const AppInjectorType *g_appInjector;

        // template <typename T>
        // auto DICreate() -> decltype(g_appInjector->template create<T>())
        // {
        //     return g_appInjector->template create<T>();
        // }

    private:
        // using InjectorType = decltype(di::make_injector(
        //     di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>(),
        //     di::bind<drogon::orm::DbClient>.to(drogon::app().getDbClient().get()),
        //     di::bind<IUserMicroserviceService>.to<UserMicroserviceService>()));

        // InjectorType injector;
        di::extension::injector<> injector{};
    };
}
