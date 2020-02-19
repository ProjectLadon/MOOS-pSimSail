/************************************************************/
/*    NAME: Pierce Nichols                                  */
/*    ORGN: Project Ladon                                   */
/*    FILE: wing.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#pragma once

#include <map>
#include <string>
#include <cmath>
#include <nlohmann/json.hpp>
#include "utility.h"
#include "wing.h"

using namespace std;

Wing::Wing(const string cl, const string cd, const string tail, const float lever, const float area) :
lever_arm(lever), sail_area(area)
{
    cl_table = loadMap(cl);
    cd_table = loadMap(cd);
    aoa_table = loadMap(tail);
}

Wing::calculate(const float windAngle, const float windSpeed, const float tail, const float density)
{
    float aoa = tableLookup(aoa_table, tail);
    float q = 0.5 * density * windSpeed * windSpeed;
    float drag = q * area * tableLookup(cd_table, aoa);
    float lift = q * area * tableLookup(cl_table, aoa);
    long_force = (lift * sin(windAngle)) - (drag * cos(windAngle));
    trans_force = -(lift * cos(windAngle)) - (drag * sin(windAngle));
    torque = trans_force * lever_arm;
}
