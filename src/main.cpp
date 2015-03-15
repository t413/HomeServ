
#include <stdio.h>
#include <iostream>
#include <fstream>

#include <microhttpd.h>
#include <jsoncpp/json/json.h>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPRequestHandler.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Util/ServerApplication.h>
#include <Poco/JSON/JSON.h>
#include <Poco/JSON/Parser.h>
#include <Poco/JSON/Stringifier.h>
#include <Poco/Dynamic/Var.h>

#include <openzwave/Options.h>
#include <openzwave/Manager.h>

using namespace Poco;
using namespace std;

class RequestHandler : public Net::HTTPRequestHandler {
    static int count;
public:
    ~RequestHandler() { cout << "ahh, I'm dying!" << endl; }
    virtual void handleRequest(Net::HTTPServerRequest &req, Net::HTTPServerResponse &resp) {
        resp.setStatus(Net::HTTPResponse::HTTP_OK);
        resp.setContentType("text/json");
        ostream& out = resp.send();
        Poco::JSON::Object ret;
        ret.set("hello", "world");
        ret.set("count", ++count);
        ret.set("host",  req.getHost());
        ret.set("method",req.getMethod());
        ret.set("uri",   req.getURI());
        JSON::Stringifier::stringify(ret, out);
        out.flush();
        cout << "Response sent for count=" << count << " and URI=" << req.getURI() << endl;
    }
};

int RequestHandler::count = 0;

class RequestHandlerFactory : public Net::HTTPRequestHandlerFactory {
    virtual Net::HTTPRequestHandler* createRequestHandler(const Net::HTTPServerRequest & req) {
        cout << "new req handler" << endl;
        return new RequestHandler;
    }
};

class Server : public Poco::Util::ServerApplication {
protected:
    int main(const vector<string> &) {
        RequestHandlerFactory factory;
        Net::HTTPServer s( &factory, Net::ServerSocket(9090), new Net::HTTPServerParams);

        s.start();
        cout << endl << "Server started" << endl;

        waitForTerminationRequest();  // wait for CTRL-C or kill

        cout << endl << "Shutting down..." << endl;
        s.stop();

        return Application::EXIT_OK;
    }
};

int main(int argc, char** argv) {
    Server app;
    return app.run(argc, argv);
}
