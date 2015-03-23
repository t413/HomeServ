#pragma once

#include <string>
#include <vector>
#include <value_classes/ValueID.h>

namespace OpenZWave { class Manager; }
namespace Poco { namespace JSON { class Object; } }


namespace home {

    typedef double LightLevel;
    double mapConstrain(double in, double in_low, double in_up, double out_low, double out_up);

    class Light {
    public:
        virtual LightLevel getLevel() = 0;
        virtual void setLevel(LightLevel level) = 0;
    };


    class ZWaveLight : public Light {
        OpenZWave::Manager *manager;
        int nodeId, homeId;
        std::vector<OpenZWave::ValueID> values;
    public:
        ZWaveLight(OpenZWave::Manager *manager, int nodeId, int homeId);
        void addValueId(OpenZWave::ValueID &v);

        // Light contract:
        LightLevel getLevel();
        void setLevel(LightLevel level);

        int getId();
        int getBtype();
        std::string getGtype();
        std::string getName();
        std::string getLocation();
        std::string getManufacturer();
        std::string getProduct();
        bool getListening();
        bool getFreqListener();
        bool getBeam();
        bool getRouting();
        bool getSecurity();
        bool getFailed();
        bool getAwake();
        std::string getState();
        //getTime()       { nodes[i]->getTime());
    };


    Light* createFromJson(Poco::JSON::Object& json);

}
