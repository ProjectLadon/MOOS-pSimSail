/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: SimSail.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#ifndef SimSail_HEADER
#define SimSail_HEADER

#include "MOOS/libMOOS/Thirdparty/AppCasting/AppCastingMOOSApp.h"
#include "MOOS/libMOOSGeodesy/MOOSGeodesy.h"
#include <string>
#include <map>

using namespace std;

/// @brief Normalizes input to the range (0.0-360.0) inclusive
/// @param float in -- The number to normalize
/// @return The normalized value
inline double normalizeHeading(double in) {
    while (in > 360.0) in -= 360.0;
    while (in < 0.0) in += 360.0;
    return in;
}

/// @brief Interpolate a value y at point x between (x0, y0) and (x1, y1)
/// @return the interpolated value, or NAN if x does not lie between x0 and x1, or x0 = x1
inline double interpolate(
    double x,
    double x0,
    double y0,
    double x1,
    double y1
) {
    if (x0 == x1) return NAN;   // If x0 and x1 are equal, interpolation is impossible
    // Check that x lies between x0 and x1
    if (x0 > x1) {
        if ((x > x0) || (x < x1)) return NAN;
    } else {
        if ((x < x0) || (x > x1)) return NAN;
    }
    double diffX = x1 - x0;
    return (((y0 * (x1 - x)) + (y1 * (x - x0)))/diffX);
}

inline double deg2rad (double deg) {return (deg*PI/180);}
inline double rad2deg (double rad) {return (rad*180/PI);}

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

    private:    // Configuration processing functions
        map<float, float> loadMap(string input);
        bool loadBoundedValue(string input, float min, float max, float &val);
        float tableLookup(map<float, float> table, float target);

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

#endif
