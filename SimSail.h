/************************************************************/
/*    NAME: Pierce Nichols                                  */
/*    ORGN: Project Ladon                                   */
/*    FILE: SimSail.h                                       */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#pragma once

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "MOOS/libMOOSGeodesy/MOOSGeodesy.h"
#include <string>
#include <map>

using namespace std;

class SimSail : public AppCastingMOOSApp {
    public:
        SimSail();
        ~SimSail();

    protected: // Standard MOOSApp functions to overload
        bool OnNewMail(MOOSMSG_LIST &NewMail);
        bool Iterate();
        bool OnConnectToServer();
        bool OnStartUp();

    protected: // Standard AppCastingMOOSApp function to overload
        bool buildReport();

    protected:
        void registerVariables();

    private:    // Physics funtions
        float getDriveForce();
        float getTurnTorque();
        float getDragForce();
        float getTurningResistance();
        float getAccel();
        float getAngularAccel();
        float getNewSpeed();
        float getNewHeading();
        void integrate();
        void publish();

    private:    // Configuration variables
        map<float, float> foresailCl;
        map<float, float> foresailCd;
        map<float, float> foresailTailMap;
        map<float, float> mizzenCl;
        map<float, float> mizzenCd;
        map<float, float> mizzenTailMap;
        float waterDrag;
        float waterTurnDrag;
        float mass;
        float turningInertia;
        float airDensity;
        float waterDensity;
        float windDirection;
        float windSpeed;
        float originLat;
        float originLon;
        float foresailArea;
        float mizzenArea;
        float foresailCgDistance;
        float mizzenCgDistance;
        float waterplaneArea;
        float waterplaneCpDistance;

    private:    // Input message names
        string foresailTailCmd;
        string mizzenTailCmd;

    private:    // Output message names
        string outputLatMsg;
        string outputLonMsg;
        string foresailHdgMsg;
        string mizzenHdgMsg;
        string boatHdgMsg;
        string speedOverGndMsg;
        string courseOverGndMsg;

    private:    // State variables
        CMOOSGeodesy    m_geo;
        float foresailTail;
        float mizzenTail;
        float foresailAoA;
        float mizzenAoA;
        float speed = 0;
        float heading = 0;
        float turnRate = 0;
        float navX = 0;
        float navY = 0;
};
