#pragma once

#include <drogon/drogon.h>
#include <drogon/plugins/Plugin.h>
#include <boost/di.hpp>
#include "boost/di/extension/scopes/shared.hpp"
#include "boost/di/extension/scopes/session.hpp"
#include "boost/di/extension/scopes/scoped.hpp"
#include "boost/di/extension/injector.hpp"
#include <iostream>
#include "../repository/IUnitOfWork.hpp"
#include "../repository/UnitOfWork.hpp"
#include "../repository/IRepository.hpp"
#include "../repository/UserMicroserviceRepository.hpp"
#include "../services/UserMicroserviceService.hpp"
#include "../services/IUserMicroserviceService.hpp"
#include "../services/ExampleDI.cpp"

#include "../services/IPcPartService.hpp"
#include "../services/PcPartService.hpp"
#include "../services/IFatService.hpp"
#include "../services/FatService.hpp"

#include "../repository/IWriterNew.hpp"
#include "../repository/StdoutWriterNew.hpp"
#include "../services/IGreeterNew.hpp"
#include "../services/GreeterImplNew.hpp"

#include <any>

using namespace app_repositories;
using namespace app_services;

namespace di = boost::di;

inline auto MakeAppInjector()
{
    return di::make_injector<di::extension::shared_config>(di::bind<Writer>().to<StdoutWriter>().in(di::extension::shared), di::bind<Greeter>().to<GreeterImpl>().in(di::extension::shared));
}

namespace drogon::plugin
{
    class DIPlugin : public drogon::Plugin<DIPlugin>
    {
    public:
        // using InjectorType = decltype(di::make_injector(
        //     di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::singleton), // UnitOfWork là singleton
        //     di::bind<IPcPartService>.to<PcPartService>().in(di::singleton),
        //     di::bind<IFatService>.to<FatService>().in(di::singleton),
        //     di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::singleton) // UserService nhận UnitOfWork
        //     ));
        // using InjectorType = di::extension::injector<>;

        DIPlugin() = default;

        /// This method must be called by drogon to initialize and start the plugin.
        /// It must be implemented by the user.
        void initAndStart(const Json::Value &config) override;

        /// This method must be called by drogon to shutdown the plugin.
        /// It must be implemented by the user.
        void shutdown() override;

        bool createSessionEx(const std::string &sessionId);
        void endSessionEx(const std::string &sessionId);

        bool createSession(const std::string &sessionId);
        void endSession(const std::string &sessionId);

        template <typename T>
        std::shared_ptr<T> get()
        {
            // auto ms = di::extension::session(_session)();
            return injector.create<std::shared_ptr<T>>();
            // return injector.create<T>();
        }

        template <typename T>
        T getUnique()
        {
            return injector.create<T>();
            // return injector.create<T>();
        }

        // template <typename T>
        // std::shared_ptr<T> getTest()
        // {
        //     auto ms = di::extension::session(_session)();
        //     return injectorTest.create<std::shared_ptr<T>>();
        //     // return injector.create<T>();
        // }

        // template <typename T>
        // T getUniqueTest()
        // {
        //     return injectorTest.create<T>();
        //     // return injector.create<T>();
        // }

        // Hàm template để lấy đối tượng từ session
        // template <typename T>
        // std::shared_ptr<T> getObjectBySession(const std::string &sessionId)
        // {
        //     auto it = sessions_.find(sessionId);
        //     if (it != sessions_.end())
        //     {
        //         return it->second->create<std::shared_ptr<T>>();
        //     }
        //     else
        //     {
        //         throw std::runtime_error("Session not found: " + sessionId);
        //     }
        // }

        // Hàm template để lấy đối tượng từ session
        // template <typename T>
        // std::shared_ptr<T> getObjectBySession(const std::string &sessionId)
        // {
        //     auto it = sessionsEx_.find(sessionId);
        //     if (it != sessionsEx_.end())
        //     {
        //         return it->second->create<std::shared_ptr<T>>();
        //     }
        //     else
        //     {
        //         throw std::runtime_error("Session not found: " + sessionId);
        //     }
        // }

    private:
        std::function<void()> _session = [] {};
        using InjectorType = decltype(di::make_injector(
            di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::extension::session(_session)), // UnitOfWork là singleton
            di::bind<IPcPartService>.to<PcPartService>().in(di::extension::session(_session)),
            di::bind<IFatService>.to<FatService>().in(di::extension::session(_session)),
            di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::extension::session(_session)) // UserService nhận UnitOfWork
            ));
        // using InjectorTypeTest = decltype(di::make_injector(di::bind<Writer>().to<StdoutWriter>().in(di::extension::shared), di::bind<Greeter>().to<GreeterImpl>().in(di::extension::shared)));
        // using InjectorTypeTest = di::extension::injector<>;

        // std::map<std::string, std::unique_ptr<InjectorType>> sessions_ = {};
        // std::map<std::string, std::shared_ptr<di::injector<std::shared_ptr<Writer>, std::shared_ptr<Greeter>>>> sessionsEx_ = {};
        // std::shared_ptr<di::injector<std::shared_ptr<Writer>, std::shared_ptr<Greeter>>> sessionsEx_[4];
        // const InjectorTypeTest *sessionsEx_;

        using InjectorTypeTest = decltype(MakeAppInjector());
        // InjectorTypeTest *sessionsEx_ = new InjectorTypeTest(MakeAppInjector());
        std::any sessionsEx_;

        // using InjectorType = decltype(di::make_injector<di::extension::shared_config>(
        //     di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::extension::shared),
        //     di::bind<IPcPartService>.to<PcPartService>().in(di::extension::shared),
        //     di::bind<IFatService>.to<FatService>().in(di::extension::shared),
        //     di::bind<app_services::IUserMicroserviceService>.to<app_services::UserMicroserviceService>().in(di::extension::shared)));

        // using InjectorType = decltype(di::make_injector(
        //     di::bind<IWriterNew>().to<StdoutWriterNew>().in(di::unique), di::bind<IGreeterNew>().to<GreeterImplNew>().in(di::unique)));

        // using InjectorType = decltype(di::make_injector(
        //     di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::unique), di::bind<IGreeterNew>().to<GreeterImplNew>().in(di::unique)));
        InjectorType injector;

        // di::injector<std::shared_ptr<unitofwork::UnitOfWork>, std::shared_ptr<IPcPartService>, std::shared_ptr<IFatService>, std::shared_ptr<app_services::IUserMicroserviceService>> injector;
        // di::extension::injector<> injector{};

        // using InjectorTypeTest = di::extension::injector<>;
        // InjectorTypeTest injectorTest;
    };
}
