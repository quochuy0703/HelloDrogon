
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

            injector = di::make_injector(
                di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::unique),  // UnitOfWork là singleton
                di::bind<IPcPartService>.to<PcPartService>().in(di::unique),
                di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::unique) // UserService nhận UnitOfWork
            );

            // injector.install(di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::unique),  // UnitOfWork là singleton
            //                  di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::unique) // UserService nhận UnitOfWork)
            // );

            // injector.install(di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::unique), di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::unique));
            // injector.install(
            //     di::bind<Writer>().to<StdoutWriter>().in(di::unique), di::bind<Greeter>().to<GreeterImpl>().in(di::unique));
            // injector = di::make_injector(di::bind<Writer>().to<StdoutWriter>().in(di::unique), di::bind<Greeter>().to<GreeterImpl>().in(di::unique));
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
}
