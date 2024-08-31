
#include "DIFruitPlugin.h"

namespace drogon::plugin
{
    void DIFruitPlugin::initAndStart(const Json::Value &config)
    {
        try
        {
            /// Initialize and start the plugin
            LOG_INFO << "DIFruitPlugin  initialized and started";
            // Cấu hình injector ở đây
            // auto dbClient = drogon::app().getDbClient();

            // auto injector = di::make_injector(
            //     di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>(),  // UnitOfWork là singleton
            //     di::bind<drogon::orm::DbClient>.to(dbClient.get()),              // Inject DbClient vào UnitOfWork
            //     di::bind<IUserMicroserviceService>.to<UserMicroserviceService>() // UserService nhận UnitOfWork
            // );

            // injector.install(di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::unique),  // UnitOfWork là singleton
            //                  di::bind<IUserMicroserviceService>.to<UserMicroserviceService>().in(di::unique) // UserService nhận UnitOfWork)
            // );

            // injector.install(di::bind<unitofwork::IUnitOfWork>.to<unitofwork::UnitOfWork>().in(di::unique));
        }
        catch (const std::runtime_error &ex)
        {
            LOG_ERROR << ex.what();
        }
    }

    void DIFruitPlugin::shutdown()
    {
        // Cleanup nếu cần
        /// Shutdown the plugin
        LOG_INFO << "DIFruitPlugin shutdown";
    }
}
