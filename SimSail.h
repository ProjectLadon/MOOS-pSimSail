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
#include "physics.h"

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

    protected:
        // operation functions
        void integrate();
        void publish();

    private:
        // Configuration variables
        BoatModel boat;
        float windDirection;
        float windSpeed;
        float originLat;
        float originLon;

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
        float navX = 0;
        float navY = 0;
};
