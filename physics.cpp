/************************************************************/
/*    NAME: Pierce Nichols                                  */
/*    ORGN: Project Ladon                                   */
/*    FILE: physics.cpp                                     */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#pragma once

#include <cmath>
#include <memory>
#include <nlohmann/json.hpp>
#include "wing.h"
#include "foil.h"
#include "utility.h"
#include "physics.h"

bool BoatModel::readConfEntry(const string key, const string value)
{
    if 
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
        skeg.getLongitudinalForce();
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