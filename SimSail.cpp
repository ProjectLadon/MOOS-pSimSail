/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: SimSail.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

// TODO Calculate true wind
// TODO Reset origin lat/lon every iteration
// TODO Double check all signs for adherence to convention
// TODO Finish app cast report

#include <iterator>
#include <map>
#include <string>
#include <cmath>
#include "MBUtils.h"
#include "ACTable.h"
#include "SimSail.h"
#include <nlohmann/json.hpp>
#include "wing.h"
#include "foil.h"
#include "utility.h"
#include "physics.h"

using namespace std;
using namespace nlohmann;   // For convenience with JSON

//---------------------------------------------------------
// Constructor

SimSail::SimSail() {
}

//---------------------------------------------------------
// Destructor

SimSail::~SimSail() {
}

//---------------------------------------------------------
// Procedure: OnNewMail

bool SimSail::OnNewMail(MOOSMSG_LIST &NewMail) {
    AppCastingMOOSApp::OnNewMail(NewMail);

    MOOSMSG_LIST::iterator p;
    for(p=NewMail.begin(); p!=NewMail.end(); p++) {
        CMOOSMsg &msg = *p;
        string key    = msg.GetKey();
        if (key == foresailTailCmd && msg.IsDouble()) {
            foresailTail = msg.GetDouble();
        } else if (key == mizzenTailCmd && msg.IsDouble()) {
            mizzenTail = msg.GetDouble();
        } else if (key != "APPCAST_REQ") {// handled by AppCastingMOOSApp
            reportRunWarning("Unhandled Mail: " + key);
        }
    }

   return(true);
}

//---------------------------------------------------------
// Procedure: OnConnectToServer

bool SimSail::OnConnectToServer() {
    registerVariables();
    return(true);
}

//---------------------------------------------------------
// Procedure: Iterate()
//            happens AppTick times per second

bool SimSail::Iterate() {
    AppCastingMOOSApp::Iterate();
    integrate();
    publish();
    AppCastingMOOSApp::PostReport();
    return(true);
}

//---------------------------------------------------------
// Procedure: OnStartUp()
//            happens before connection is open

bool SimSail::OnStartUp() {
    AppCastingMOOSApp::OnStartUp();

    STRING_LIST sParams;
    m_MissionReader.EnableVerbatimQuoting(false);
    if(!m_MissionReader.GetConfiguration(GetAppName(), sParams))
    reportConfigWarning("No config block found for " + GetAppName());

    STRING_LIST::iterator p;
    for(p=sParams.begin(); p!=sParams.end(); p++) {
        string orig  = *p;
        string line  = *p;
        string param = toupper(biteStringX(line, '='));
        string value = line;

        bool handled = false;
        if (param == "WIND_DIRECTION") {
            handled = loadBoundedValue(value, 0, 360, windDirection);
        } else if (param == "WIND_SPEED") {
            handled = loadBoundedValue(value, 0, 100, windSpeed);
        } else if (param == "ORIGIN_LAT") {
            handled = loadBoundedValue(value, -90, 90, originLat);
        } else if (param == "ORIGIN_LON") {
            handled = loadBoundedValue(value, -180, 180, originLon);
        } else if (param == "START_X") {
            navX = stof(value);
            handled = true;
        } else if (param == "START_Y") {
            navY = stof(value);
            handled = true;
        } else if (param == "FORESAIL_TAIL_CMD") {
            foresailTailCmd = value;
            handled = true;
        } else if (param == "MIZZEN_TAIL_CMD") {
            mizzenTailCmd = value;
            handled = true;
        } else if (param == "OUTPUT_LAT_MSG") {
            outputLatMsg = value;
            handled = true;
        } else if (param == "OUTPUT_LON_MSG") {
            outputLonMsg = value;
            handled = true;
        } else if (param == "FORESAIL_HDG_MSG") {
            foresailHdgMsg = value;
            handled = true;
        } else if (param == "MIZZEN_HDG_MSG") {
            mizzenHdgMsg = value;
            handled = true;
        } else if (param == "BOAT_HDG_MSG") {
            boatHdgMsg = value;
            handled = true;
        } else if (param == "SPEED_OVER_GND_MSG") {
            speedOverGndMsg = value;
            handled = true;
        } else if (param == "COURSE_OVER_GND_MSG") {
            courseOverGndMsg = value;
            handled = true;
        } else {
            handled = boat.readConfEntry(param, value);
        }

        if(!handled) reportUnhandledConfigWarning(orig);

    }

    registerVariables();
    m_geo.Initialise(originLat, originLon);
    return(true);
}

//---------------------------------------------------------
// Procedure: registerVariables

void SimSail::registerVariables() {
    AppCastingMOOSApp::RegisterVariables();
    Register(foresailTailCmd, 0);
    Register(mizzenTailCmd, 0);
}

void SimSail::integrate() {
    float windAngle = deg2rad_f(normalizeHeading(windDirection - boat.getHeading()));
    float dt = (1.0/GetAppFreq());
    boat.calculate(
        windAngle,
        windSpeed,
        foresailTail,
        mizzenTail,
        dt
    );
    navX += boat.getSpeed() * dt * sin(deg2rad_f(boat.getCourseOverGround()));
    navY += boat.getSpeed() * dt * cos(deg2rad_f(boat.getCourseOverGround()));
}

void SimSail::publish() {
    double lat, lon;
    float windAngle = deg2rad_f(normalizeHeading(windDirection - boat.getHeading()));
    m_geo.LocalGrid2LatLong(navX, navY, lat, lon);
    Notify(outputLatMsg, lat);
    Notify(outputLonMsg, lon);
    Notify(foresailHdgMsg, boat.getForeWing()->getSailHeading(boat.getHeading(), windAngle, foresailTail));
    Notify(mizzenHdgMsg, boat.getMizzen()->getSailHeading(boat.getHeading(), windAngle, mizzenTail));
    Notify(boatHdgMsg, boat.getHeading());
    Notify(speedOverGndMsg, boat.getSpeed());
    Notify(courseOverGndMsg, boat.getCourseOverGround());
}

//------------------------------------------------------------
// Procedure: buildReport()

bool SimSail::buildReport()
{
  m_msgs << "============================================ \n";
  m_msgs << "File:                                        \n";
  m_msgs << "============================================ \n";

  ACTable actab(4);
  actab << "Alpha | Bravo | Charlie | Delta";
  actab.addHeaderLines();
  actab << "one" << "two" << "three" << "four";
  m_msgs << actab.getFormattedString();

  return(true);
}
