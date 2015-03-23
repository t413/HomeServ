#include "Light.h"

#include <Manager.h>

using namespace std;

namespace home {

    double mapConstrain(double in, double in_low, double in_up, double out_low, double out_up) {
        double out = (in > in_up)? in_up : ((in < in_low)? in_low : in); //constrain
        return (out - in_low) * (out_up - out_low) / (in_up - in_low) +out_low;
    }

    /* -------------------- */
    /* ---- ZWaveLight ---- */
    /* -------------------- */

    ZWaveLight::ZWaveLight(OpenZWave::Manager* manager, int nodeId, int homeId)
        : manager(manager), nodeId(nodeId), homeId(homeId) {{}
    }

    LightLevel ZWaveLight::getLevel() {
    }

    void ZWaveLight::setLevel(LightLevel level) {
        manager->SetNodeLevel(homeId,nodeId,level);
    }

    int    ZWaveLight::getId()           { return nodeId; }
    int    ZWaveLight::getBtype()        { return manager->GetNodeBasic(homeId, nodeId); }
    string ZWaveLight::getGtype()        { return manager->GetNodeType(homeId, nodeId); }
    string ZWaveLight::getName()         { return manager->GetNodeName(homeId, nodeId); }
    string ZWaveLight::getLocation()     { return manager->GetNodeLocation(homeId, nodeId); }
    string ZWaveLight::getManufacturer() { return manager->GetNodeManufacturerName(homeId, nodeId); }
    string ZWaveLight::getProduct()      { return manager->GetNodeProductName(homeId, nodeId); }
    bool   ZWaveLight::getListening()    { return manager->IsNodeListeningDevice(homeId, nodeId); }
    bool   ZWaveLight::getFreqListener() { return manager->IsNodeFrequentListeningDevice(homeId, nodeId); }
    bool   ZWaveLight::getBeam()         { return manager->IsNodeBeamingDevice(homeId, nodeId); }
    bool   ZWaveLight::getRouting()      { return manager->IsNodeRoutingDevice(homeId, nodeId); }
    bool   ZWaveLight::getSecurity()     { return manager->IsNodeSecurityDevice(homeId, nodeId); }
    bool   ZWaveLight::getFailed()       { return manager->IsNodeFailed(homeId, nodeId); }
    bool   ZWaveLight::getAwake()        { return manager->IsNodeAwake(homeId, nodeId); }
    string ZWaveLight::getState()        { return manager->GetNodeQueryStage(homeId, nodeId); }

}