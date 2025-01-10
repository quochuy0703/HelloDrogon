
#include "DIFruitPlugin.h"

namespace drogon::plugin
{
    Component<app_services::IUserMicroserviceService, IPcPartService, IFatService, ISystemSerialDetailService> getFruitComponent()
    {

        return fruit::createComponent().bind<unitofwork::IUnitOfWork, unitofwork::UnitOfWork>().bind<IPcPartService, PcPartService>().bind<app_services::IUserMicroserviceService, app_services::UserMicroserviceService>().bind<IFatService, FatService>().bind<ISystemSerialDetailService, SystemSerialDetailService>();
    }

    void DIFruitPlugin::initAndStart(const Json::Value &config)
    {
        try
        {

            LOG_INFO << "DIFruitPlugin  initialized and started";
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
    bool DIFruitPlugin::createSession(const std::string &sessionId)
    {
        auto injector = std::make_shared<InjectorType>(getFruitComponent);
        // std::cout << sizeof(injector) << std::endl;
        // sessions_[sessionId] = std::move(injector);

        sessions_.emplace(sessionId, std::move(injector));
        LOG_INFO << "Session start: " << sessionId << ". Total sessions_ current: " << sessions_.size();
        return true;
    }
    bool DIFruitPlugin::endSession(const std::string &sessionId)
    {
        sessions_.erase(sessionId);
        std::cout << "Session end: " << sessionId << std::endl;
        return true;
    }

}
