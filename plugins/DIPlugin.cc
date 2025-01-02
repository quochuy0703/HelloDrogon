
#include "DIPlugin.h"

namespace drogon::plugin
{
    void DIPlugin::initAndStart(const Json::Value &config)
    {
        try
        {
            /// Initialize and start the plugin
            LOG_INFO << "DIPlugin  initialized and started";

            // Cấu hình injector ở đây

            // injector = di::make_injector(di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::extension::shared), // UnitOfWork là singleton
            //                              di::bind<IPcPartService>.to<PcPartService>().in(di::extension::shared)
            //                              //  di::bind<IFatService>.to<FatService>().in(di::extension::shared),
            //                              //  di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::extension::shared)
            // );

            // injector = di::make_injector(di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::extension::session(_session)), // UnitOfWork là singleton
            //                              di::bind<IPcPartService>.to<PcPartService>().in(di::extension::session(_session)),
            //                              di::bind<IFatService>.to<FatService>().in(di::extension::session(_session)),
            //                              di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::extension::session(_session)) // UserService nhận UnitOfWork
            // );

            injector = di::make_injector(di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::extension::session(_session)), // UnitOfWork là singleton
                                         di::bind<IPcPartService>.to<PcPartService>().in(di::extension::session(_session)),
                                         di::bind<IFatService>.to<FatService>().in(di::extension::session(_session)),
                                         di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::extension::session(_session)) // UserService nhận UnitOfWork
            );

            // injector = di::make_injector<di::extension::shared_config>(di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::extension::shared), // UnitOfWork là singleton
            //                                                            di::bind<IPcPartService>.to<PcPartService>().in(di::extension::shared),
            //                                                            di::bind<IFatService>.to<FatService>().in(di::extension::shared),
            //                                                            di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::extension::shared) // UserService nhận UnitOfWork
            // );

            // injector.install(di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::unique),  // UnitOfWork là singleton
            //                  di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::unique) // UserService nhận UnitOfWork)
            // );

            // injector.install(di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::unique), di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::unique));
            // injector.install(
            //     di::bind<Writer>().to<StdoutWriter>().in(di::unique), di::bind<Greeter>().to<GreeterImpl>().in(di::unique));

            // injectorTest = di::make_injector(di::bind<Writer>().to<StdoutWriter>().in(di::unique), di::bind<Greeter>().to<GreeterImpl>().in(di::unique));

            // injectorTest = di::make_injector(di::bind<Writer>().to<StdoutWriter>().in(di::extension::shared), di::bind<Greeter>().to<GreeterImpl>().in(di::extension::shared));

            // injector = di::make_injector(di::bind<IWriterNew>().to<StdoutWriterNew>().in(di::unique), di::bind<IGreeterNew>().to<GreeterImplNew>().in(di::unique));
            // injector = di::make_injector(di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::unique), di::bind<IGreeterNew>().to<GreeterImplNew>().in(di::unique));
        }
        catch (const std::runtime_error &ex)
        {
            LOG_ERROR << ex.what();
        }
    }

    void DIPlugin::shutdown()
    {
        // Cleanup nếu cần
        /// Shutdown the plugin
        LOG_INFO << "DIPlugin shutdown";
    }

    bool DIPlugin::createSession(const std::string &sessionId)
    {
        // auto injector = std::make_unique<DIPlugin::InjectorType>(di::make_injector(di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::extension::shared), // UnitOfWork là singleton
        //                                                                                                                                                                      //  di::bind<IPcPartService>.to<PcPartService>().in(di::extension::shared),
        //                                                                                                                                                                      //  di::bind<IFatService>.to<FatService>().in(di::extension::shared),
        //                                                                            di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::extension::shared)));
        // sessions_[sessionId] = std::move(injector);
        // std::cout << "Session created: " << sessionId << '\n';
        // return true;

        return true;
    }
    void DIPlugin::endSession(const std::string &sessionId)
    {
        // auto ptr = std::move(sessions_[sessionId]);
        // ptr.reset();
        // // auto injector = ptr.get();
        // // LOG_INFO << "endSession ptr count: " << ptr.use_count();
        // sessions_.erase(sessionId);
        // // LOG_INFO << "endSession ptr count: " << ptr.use_count();

        // std::cout << "Session ended for: " << sessionId << '\n';
    }
    // std::shared_ptr<di::extension::detail::session<drogon::AdviceChainCallback, di::scopes::singleton>> DIPlugin::createSession()
    // {
    //     auto ms = std::make_shared<di::extension::detail::session<drogon::AdviceChainCallback, di::scopes::singleton>>(di::extension::session(_session));
    //     // Cấu hình injector ở đây
    //     // injector = di::make_injector<di::extension::shared_config>(
    //     //     di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::extension::session(_session)), // UnitOfWork là singleton
    //     //     di::bind<IPcPartService>.to<PcPartService>().in(di::extension::session(_session)),
    //     //     di::bind<IFatService>.to<FatService>().in(di::extension::session(_session)),
    //     //     di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::extension::session(_session)) // UserService nhận UnitOfWork
    //     // );
    //     // injector = di::make_injector<di::extension::shared_config>(
    //     //     di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::extension::shared), // UnitOfWork là singleton
    //     //     di::bind<IPcPartService>.to<PcPartService>().in(di::extension::shared),
    //     //     di::bind<IFatService>.to<FatService>().in(di::extension::shared),
    //     //     di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::extension::shared) // UserService nhận UnitOfWork
    //     // );
    //     return ms;
    // }

    bool DIPlugin::createSessionEx(const std::string &sessionId)
    {
        // Tạo shared scope

        // auto injector = std::make_shared<di::extension::injector<>>(di::make_injector(di::bind<Writer>().to<StdoutWriter>().in(di::extension::shared), di::bind<Greeter>().to<GreeterImpl>().in(di::extension::shared)));
        // auto injector = std::make_shared<di::injector<std::shared_ptr<Writer>, std::shared_ptr<Greeter>>>(di::make_injector<di::extension::shared_config>(di::bind<Writer>().to<StdoutWriter>().in(di::extension::shared), di::bind<Greeter>().to<GreeterImpl>().in(di::extension::shared)));
        DIPlugin::InjectorTypeTest injector = MakeAppInjector();
        LOG_INFO << sizeof(sessionsEx_);
        // sessionsEx_[sessionId] = std::move(injector);
        this->sessionsEx_ = std::move(&injector);
        // sessionsEx_.emplace(
        //     sessionId,
        //     injector);

        // std::cout
        //     << "Session created: " << sessionId << '\n';
        return true;
    }
    void DIPlugin::endSessionEx(const std::string &sessionId)
    {
        // sessionsEx_.erase(sessionId);
        std::cout << "Session ended for: " << sessionId << '\n';
    }
}
