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

//openzwave:
#include <Options.h>
#include <Manager.h>
#include <Notification.h>

#include "Light.h"

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

    void ServerApp::initialize(Poco::Util::Application& app) {
        manager = OpenZWave::Manager::Create();
        manager->AddWatcher(ServerApp::OnNotification, this);
    }

    void ServerApp::OnNotification (const OpenZWave::Notification* n, void* context) {
        ServerApp* app = static_cast<ServerApp*>(context);
        OpenZWave::ValueID id = n->GetValueID();
        const string& type = notifyTypeMap.at(n->GetType());
        app->logger().notice("ServerApp::OnNotification type:" + type);
        if (type == "NodeAdded") {
            app->lights.push_back(new home::ZWaveLight(app->manager, n->GetNodeId(), n->GetHomeId()));
        }
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


    const map<OpenZWave::Notification::NotificationType, const string> ServerApp::notifyTypeMap = {
            { OpenZWave::Notification::Type_ValueAdded ,      "ValueAdded" },       // A new node value has been added to OpenZWave's list. These notifications occur after a node has been discovered, and details of its command classes have been received.  Each command class may generate one or more values depending on the complexity of the item being represented.
            { OpenZWave::Notification::Type_ValueRemoved,     "ValueRemoved" },     // A node value has been removed from OpenZWave's list.  This only occurs when a node is removed.
            { OpenZWave::Notification::Type_ValueChanged,     "ValueChanged" },     // A node value has been updated from the Z-Wave network and it is different from the previous value.
            { OpenZWave::Notification::Type_ValueRefreshed,   "ValueRefreshed" },   // A node value has been updated from the Z-Wave network.
            { OpenZWave::Notification::Type_Group,            "Group" },            // The associations for the node have changed. The application should rebuild any group information it holds about the node.
            { OpenZWave::Notification::Type_NodeNew,          "NodeNew" },          // A new node has been found (not already stored in zwcfg*.xml file)
            { OpenZWave::Notification::Type_NodeAdded,        "NodeAdded" },        // A new node has been added to OpenZWave's list.  This may be due to a device being added to the Z-Wave network, or because the application is initializing itself.
            { OpenZWave::Notification::Type_NodeRemoved,      "NodeRemoved" },      // A node has been removed from OpenZWave's list.  This may be due to a device being removed from the Z-Wave network, or because the application is closing.
            { OpenZWave::Notification::Type_NodeProtocolInfo, "NodeProtocolInfo" }, // Basic node information has been receievd, such as whether the node is a listening device, a routing device and its baud rate and basic, generic and specific types. It is after this notification that you can call Manager::GetNodeType to obtain a label containing the device description.
            { OpenZWave::Notification::Type_NodeNaming,       "NodeNaming" },       // One of the node names has changed (name, manufacturer, product).
            { OpenZWave::Notification::Type_NodeEvent,        "NodeEvent" },        // A node has triggered an event.  This is commonly caused when a node sends a Basic_Set command to the controller.  The event value is stored in the notification.
            { OpenZWave::Notification::Type_PollingDisabled,  "PollingDisabled" },  // Polling of a node has been successfully turned off by a call to Manager::DisablePoll
            { OpenZWave::Notification::Type_PollingEnabled,   "PollingEnabled" },   // Polling of a node has been successfully turned on by a call to Manager::EnablePoll
            { OpenZWave::Notification::Type_SceneEvent,       "SceneEvent" },       // Scene Activation Set received
            { OpenZWave::Notification::Type_CreateButton,     "CreateButton" },     // Handheld controller button event created
            { OpenZWave::Notification::Type_DeleteButton,     "DeleteButton" },     // Handheld controller button event deleted
            { OpenZWave::Notification::Type_ButtonOn,         "ButtonOn" },         // Handheld controller button on pressed event
            { OpenZWave::Notification::Type_ButtonOff,        "ButtonOff" },        // Handheld controller button off pressed event
            { OpenZWave::Notification::Type_DriverReady,      "DriverReady" },      // A driver for a PC Z-Wave controller has been added and is ready to use.  The notification will contain the controller's Home ID, which is needed to call most of the Manager methods.
            { OpenZWave::Notification::Type_DriverFailed,     "DriverFailed" },     // Driver failed to load
            { OpenZWave::Notification::Type_DriverReset,      "DriverReset" },      // All nodes and values for this driver have been removed.  This is sent instead of potentially hundreds of individual node and value notifications.
            { OpenZWave::Notification::Type_EssentialNodeQueriesComplete, "EssentialNodeQueriesComplete" }, // The queries on a node that are essential to its operation have been completed. The node can now handle incoming messages.
            { OpenZWave::Notification::Type_NodeQueriesComplete,          "NodeQueriesComplete" },          // All the initialisation queries on a node have been completed.
            { OpenZWave::Notification::Type_AwakeNodesQueried  ,          "AwakeNodesQueried" },            // All awake nodes have been queried, so client application can expected complete data for these nodes.
            { OpenZWave::Notification::Type_AllNodesQueriedSomeDead,      "AllNodesQueriedSomeDead" },      // All nodes have been queried but some dead nodes found.
            { OpenZWave::Notification::Type_AllNodesQueried,              "AllNodesQueried" },              // All nodes have been queried, so client application can expected complete data.
            { OpenZWave::Notification::Type_Notification,                 "Notification" },                 // An error has occured that we need to report.
            { OpenZWave::Notification::Type_DriverRemoved,                "DriverRemoved" },                // The Driver is being removed. (either due to Error or by request) Do Not Call Any Driver Related Methods after recieving this call
    };
}