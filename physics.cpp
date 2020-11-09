/************************************************************/
/*    NAME: Pierce Nichols                                  */
/*    ORGN: Project Ladon                                   */
/*    FILE: physics.cpp                                     */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <cmath>
#include <memory>
#include <nlohmann/json.hpp>
#include <sstream>
#include "ACTable.h"
#include "wing.h"
#include "foil.h"
#include "utility.h"
#include "physics.h"

bool BoatModel::readConfEntry(const string key, const string value)
{
    if (key == "FORESAIL_CL") {
        forewing.setCl(value);
    } else if (key == "FORESAIL_CD") {
        forewing.setCd(value);
    } else if (key == "FORESAIL_TAIL") {
        forewing.setTail(value);
    } else if (key == "FORESAIL_AREA") {
        float val;
        if (loadBoundedValue(value, 0, 100, val)) {
            forewing.setArea(val);
        } else return false;
    } else if (key == "FORESAIL_CG_DISTANCE") {
        float val;
        if (loadBoundedValue(value, -100, 100, val)) {
            forewing.setLever(val);
        } else return false;
    } else if (key == "MIZZEN_CL") {
        forewing.setCl(value);
    } else if (key == "MIZZEN_CD") {
        forewing.setCd(value);
    } else if (key == "MIZZEN_TAIL") {
        forewing.setTail(value);
    } else if (key == "MIZZEN_AREA") {
        float val;
        if (loadBoundedValue(value, 0, 100, val)) {
            forewing.setArea(val);
        } else return false;
    } else if (key == "MIZZEN_CG_DISTANCE") {
        float val;
        if (loadBoundedValue(value, -100, 100, val)) {
            forewing.setLever(val);
        } else return false;
    } else if (key == "WATER_DRAG") {
        float val;
        if (loadBoundedValue(value, 0, 1000, val)) {
            water_drag = val;
        } else return false;
    } else if (key == "INERTIA_MOMENT") {
        float val;
        if (loadBoundedValue(value, 0, 1000, val)) {
            inertia_moment = val;
        } else return false;
    } else if (key == "MASS") {
        float val;
        if (loadBoundedValue(value, 0, 10000, val)) {
            mass = val;
        } else return false;
    } else if (key == "KEEL_AREA") {
        float val;
        if (loadBoundedValue(value, 0, 10000, val)) {
            keel.setArea(val);
        } else return false;
    } else if (key == "KEEL_CG_DISTANCE") {
        float val;
        if (loadBoundedValue(value, -100, 100, val)) {
            keel.setLever(val);
        } else return false;
    } else if (key == "SKEG_AREA") {
        float val;
        if (loadBoundedValue(value, 0, 10000, val)) {
            skeg.setArea(val);
        } else return false;
    } else if (key == "SKEG_CG_DISTANCE") {
        float val;
        if (loadBoundedValue(value, -100, 100, val)) {
            skeg.setLever(val);
        } else return false;
    } else if (key == "AIR_DENSITY") {
        float val;
        if (loadBoundedValue(value, 0.5, 5, val)) {
            air_density = val;
        } else return false;
    } else if (key == "WATER_DENSITY") {
        float val;
        if (loadBoundedValue(value, 800, 2000, val)) {
            water_density = val;
        } else return false;
    } else {return false;}
    return true;
}

void BoatModel::calculate(const float windAngle, const float windSpeed, const float foretail, const float mizzentail, const float dt)
{
    // calculate force and torque contributions
    float water_speed = sqrt((long_velocity * long_velocity) + (trans_velocity * trans_velocity));
    forewing.calculate(windAngle, windSpeed, foretail, air_density);
    mizzen.calculate(windAngle, windSpeed, mizzentail, air_density);
    keel.calculate(theta, water_speed, water_density);
    skeg.calculate(theta, water_speed, water_density);

    // sum forces and torques
    float long_force =
        forewing.getLongitudinalForce() +
        mizzen.getLongitudinalForce() +
        keel.getLongitudinalForce() +
        skeg.getLongitudinalForce() -
        (water_drag * 0.5 * water_density * long_velocity * long_velocity);
    float trans_force =
        forewing.getTransverseForce() +
        mizzen.getTransverseForce() +
        keel.getTransverseForce() +
        skeg.getTransverseForce();
    float torque =
        forewing.getTorque() +
        mizzen.getTorque() +
        keel.getTorque() +
        skeg.getTorque();

    // update state variables
    long_velocity   += (long_force/mass) * dt;
    trans_velocity  += (trans_force/mass) * dt;
    omega           += (torque/inertia_moment) * dt;
    heading         += omega * dt;
    theta           = atan2(trans_velocity, long_velocity);
}

string BoatModel::buildReport()
{
    ACTable params(5);
    ACTable state(5);
    state << "V(x), m/s | V(y), m/s | omega, rad/s | theta, rad | heading, deg";
    params << "A*Cd(water) | M, kg | Izz, kg-m^2 | rho(water) kg/m^3 | rho(air) kg/m^3";
    state.addHeaderLines();
    params.addHeaderLines();
    params << to_string(water_drag);
    params << to_string(mass);
    params << to_string(inertia_moment);
    params << to_string(water_density);
    params << to_string(air_density);
    state << to_string(long_velocity);
    state << to_string(trans_velocity);
    state << to_string(omega);
    state << to_string(theta);
    state << to_string(heading);

    stringstream result;
    result << "=================Boat State=================" << endl;
    result << params.getFormattedString() << endl << endl;
    result << state.getFormattedString() << endl;
    result << "==================Foresail==================" << endl;
    result << forewing.buildReport() << endl;
    result << "===================Mizzen===================" << endl;
    result << mizzen.buildReport() << endl;
    result << "====================Keel====================" << endl;
    result << keel.buildReport() << endl;
    result << "====================Skeg====================" << endl;
    result << skeg.buildReport() << endl;
    return result.str();
}
