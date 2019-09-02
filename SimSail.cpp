/************************************************************/
/*    NAME:                                               */
/*    ORGN: MIT                                             */
/*    FILE: SimSail.cpp                                        */
/*    DATE:                                                 */
/************************************************************/

#include <iterator>
#include <map>
#include <string>
#include <cmath>
#include "MBUtils.h"
#include "ACTable.h"
#include "SimSail.h"
#include <nlohmann/json.hpp>

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
            foresailAoA = tableLookup(foresailTailMap, foresailTail);
        } else if (key == mizzenTailCmd && msg.IsDouble()) {
            mizzenTail = msg.GetDouble();
            mizzenAoA = tableLookup(mizzenTailMap, mizzenTail);
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
    getNewSpeed();
    getNewHeading();
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
        if (param == "FORESAIL_CL") {
            foresailCl = loadMap(value);
            handled = true;
        } else if (param == "FORESAIL_CD") {
            foresailCd = loadMap(value);
            handled = true;
        } else if (param == "MIZZEN_CL") {
            mizzenCl = loadMap(value);
            handled = true;
        } else if (param == "MIZZEN_CD") {
            mizzenCd = loadMap(value);
            handled = true;
        } else if (param == "FORESAIL_TAIL") {
            foresailTailMap = loadMap(value);
            handled = true;
        } else if (param == "MIZZEN_TAIL") {
            mizzenTailMap = loadMap(value);
            handled = true;
        } else if (param == "FORESAIL_AREA") {
            handled = loadBoundedValue(value, 0, 100, foresailArea);
        } else if (param == "MIZZEN_AREA") {
            handled = loadBoundedValue(value, 0, 100, mizzenArea);
        } else if (param == "FORESAIL_CG_DISTANCE") {
            handled = loadBoundedValue(value, 0, 20, foresailCgDistance);
        } else if (param == "MIZZEN_CG_DISTANCE") {
            handled = loadBoundedValue(value, 0, 20, mizzenCgDistance);
        }  else if (param == "WATER_DRAG") {
            handled = loadBoundedValue(value, 0, 10000, waterDrag);
        } else if (param == "WATER_TURNING_DRAG") {
            handled = loadBoundedValue(value, 0, 10000, waterTurnDrag);
        } else if (param == "WATER_DENSITY") {
            handled = loadBoundedValue(value, 0, 100000, waterDensity);
        } else if (param == "AIR_DENSITY") {
            handled = loadBoundedValue(value, 0, 1000, airDensity);
        } else if (param == "MASS") {
            handled = loadBoundedValue(value, 0, 1000, mass);
        } else if (param == "TURNING_INERTIA") {
            handled = loadBoundedValue(value, 0, 1000, turningInertia);
        } else if (param == "WIND_DIRECTION") {
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

map<float, float> SimSail::loadMap(string input) {
    json in = json::parse(input);
    map<float, float> result;
    if (in.is_array()) {
        for (auto &a: in) {
            if (a.is_array() && a[0].is_number() && a[1].is_number()) {
                result.emplace(a[0].get<float>(), a[1].get<float>());
            }
        }
    }
    return result;
}

bool SimSail::loadBoundedValue(string input, float min, float max, float &val) {
    float in = stof(input);
    if (in < min) return false;
    if (in > max) return false;
    val = in;
    return true;
}

float SimSail::tableLookup(map<float, float> table, float target) {
    auto top = table.cbegin();
    auto bot = table.cbegin();
    float result;
    // Note that this loop depends on the fact that map keys are sorted
    for (auto it = table.cbegin(); it != table.cend(); ++it) {
        if (it->first <= target) bot = it;
        if (it->first >= target) {
            top = it;
            break;
        }
    }
    if (top->first == bot->first) {
        result = top->second;
    } else {
        result = interpolate(target, bot->first, bot->second, top->first, top->second);
    }
    return result;
}

float SimSail::getDriveForce() {
    float q = 0.5 * airDensity * windSpeed * windSpeed;
    float forelift = q * foresailArea * tableLookup(foresailCl, foresailAoA);
    float foredrag = q * foresailArea * tableLookup(foresailCd, foresailAoA);
    float mizzenlift = q * mizzenArea * tableLookup(mizzenCl, mizzenAoA);
    float mizzendrag = q * mizzenArea * tableLookup(mizzenCd, mizzenAoA);
    return (forelift * sin(windDirection) - foredrag * cos(windDirection)) +
        (mizzenlift * sin(windDirection) - mizzendrag * cos(windDirection));
}

float SimSail::getTurnTorque() {
    float q = 0.5 * airDensity * windSpeed * windSpeed;
    float forelift = q * foresailArea * tableLookup(foresailCl, foresailAoA);
    float foredrag = q * foresailArea * tableLookup(foresailCd, foresailAoA);
    float mizzenlift = q * mizzenArea * tableLookup(mizzenCl, mizzenAoA);
    float mizzendrag = q * mizzenArea * tableLookup(mizzenCd, mizzenAoA);
    float foretorque = -foresailCgDistance * (forelift * cos(windDirection) +
                        foredrag * sin(windDirection));
    float mizzentorque = mizzenCgDistance * (mizzenlift * cos(windDirection) +
                        mizzendrag * sin(windDirection));
    return foretorque + mizzentorque;
}

float SimSail::getDragForce() {
    return (waterDensity * waterDrag * speed * speed);
}

float SimSail::getTurningResistance() {
    return -(waterDensity * waterTurnDrag * turnRate * turnRate);
}

float SimSail::getAccel() {
    return (getDriveForce() - getDragForce())/mass;
}

float SimSail::getAngularAccel() {
    return (getTurnTorque() - getTurningResistance())/turningInertia;
}

float SimSail::getNewSpeed() {
    speed += getAccel() * (1.0/GetAppFreq());
    return speed;
}

float SimSail::getNewHeading() {
    turnRate += getAngularAccel();
    heading += rad2deg(turnRate) * (1.0/GetAppFreq());
    heading = normalizeHeading(heading);
    return heading;
}

void SimSail::integrate() {
    navX += speed * (1.0/GetAppFreq()) * sin(deg2rad(heading));
    navY += speed * (1.0/GetAppFreq()) * cos(deg2rad(heading));
}

void SimSail::publish() {
    double lat, lon;
    m_geo.LocalGrid2LatLong(navX, navY, lat, lon);
    Notify(outputLatMsg, lat);
    Notify(outputLonMsg, lon);
    Notify(foresailHdgMsg, (windDirection - foresailAoA));
    Notify(mizzenHdgMsg, (windDirection - mizzenAoA));
    Notify(boatHdgMsg, heading);
    Notify(speedOverGndMsg, speed);
    Notify(courseOverGndMsg, heading);
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
