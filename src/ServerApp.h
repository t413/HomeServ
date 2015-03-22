#pragma once

#include <vector>
#include <string>
#include <Poco/Util/ServerApplication.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPRequestHandlerFactory.h>

//Forward declarations:
namespace Poco {
    namespace Net{
        class HTTPServerRequest;
        class HTTPServerResponse;
    }
}

namespace serv {

    class ServerApp : public Poco::Util::ServerApplication {
        void defineOptions(Poco::Util::OptionSet& options);
        void printProperties(const std::string& base);
        int main(const std::vector<std::string> &);
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
