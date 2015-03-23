#pragma once

#include <vector>
#include <string>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>
#include <Notification.h>

//Forward declarations:
namespace OpenZWave {
    class Manager;
    class Notification;
}
namespace home {
    class Light;
}

namespace serv {

    class ServerApp : public Poco::Util::ServerApplication {
        OpenZWave::Manager* manager;
        static const std::map<OpenZWave::Notification::NotificationType , const std::string> notifyTypeMap;
        static void OnNotification (const OpenZWave::Notification* n, void* context);

        void defineOptions(Poco::Util::OptionSet& options);
        void printProperties(const std::string& base);
        void initialize(Application& app);
        int main(const std::vector<std::string> &);

        std::vector<home::Light*> lights;
    };

    class RequestHandlerFactory : public Poco::Net::HTTPRequestHandlerFactory {
        ServerApp& app;
        virtual Poco::Net::HTTPRequestHandler *createRequestHandler(const Poco::Net::HTTPServerRequest &req);
    public:
        RequestHandlerFactory(ServerApp &app);
    };

    class RequestHandler : public Poco::Net::HTTPRequestHandler {
        serv::ServerApp& app;
    public:
        virtual void handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp);
        RequestHandler(ServerApp &app);
    };

}
