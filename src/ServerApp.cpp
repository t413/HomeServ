//
// Created by Tim O'Brien on 3/21/15.
//

#include "ServerApp.h"

#include <iostream>
#include <fstream>
#include <Poco/Net/HTTPServer.h>
#include <Poco/Net/HTTPServerResponse.h>
#include <Poco/Net/HTTPServerRequest.h>
#include <Poco/JSON/Parser.h>

#include <openzwave/Options.h>
#include <openzwave/Manager.h>

using namespace std;
using namespace Poco;
using namespace Poco::Net;


namespace serv {

    /* ------------------- */
    /* ---- ServerApp ---- */
    /* ------------------- */

    void ServerApp::defineOptions(Util::OptionSet& options){
        options.addOption(Util::Option("port","p","bind to port")
                .argument("port",true).binding("config.port"));
        options.addOption(Util::Option("file","f","use config file")
                .argument("file",true).binding("config.file"));
        Application::defineOptions(options);
    }


    void ServerApp::printProperties(const std::string& base) {
        Util::AbstractConfiguration::Keys keys;
        config().keys(base, keys);
        if (keys.empty()) {
            if (config().hasProperty(base))
                logger().information(base + " = " + config().getString(base));
        } else {
            for (const string &k : keys) {
                printProperties((base.empty())? k : base + '.' + k);
            }
        }
    }

    int ServerApp::main(const vector<string> &) {
        RequestHandlerFactory factory(*this);
        int port = config().getInt("config.port", 9090);
        Net::HTTPServer s( &factory, Net::ServerSocket(port), new Net::HTTPServerParams);

        s.start();
        string file = config().getString("config.file", "nope!");
        printProperties("");

        cout << endl << "Server started, " << file << ", port " << port << endl;

        waitForTerminationRequest();  // wait for CTRL-C or kill

        cout << endl << "Shutting down..." << endl;
        s.stop();

        return Application::EXIT_OK;
    }

    /* ------------------ */
    /* ---- Handlers ---- */
    /* ------------------ */

    RequestHandler::RequestHandler(ServerApp &app) : app(app) {

    }

    RequestHandlerFactory::RequestHandlerFactory(ServerApp &app) : app(app) {

    }

    Poco::Net::HTTPRequestHandler* RequestHandlerFactory::createRequestHandler(const Poco::Net::HTTPServerRequest &req) {
        cout << "new req handler" << endl;
        return new RequestHandler(app);
    }

    void RequestHandler::handleRequest(Poco::Net::HTTPServerRequest &req, Poco::Net::HTTPServerResponse &resp) {
        resp.setStatus(Net::HTTPResponse::HTTP_OK);
        resp.setContentType("text/json");
        ostream &out = resp.send();
        Poco::JSON::Object ret;
        ret.set("hello", "world");
        ret.set("host", req.getHost());
        ret.set("method", req.getMethod());
        ret.set("uri", req.getURI());
        JSON::Stringifier::stringify(ret, out);
        out.flush();

        app.logger().notice("Response sent URI=" + req.getURI());
    }
}