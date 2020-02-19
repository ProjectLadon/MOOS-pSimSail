/************************************************************/
/*    NAME: Pierce Nichols                                  */
/*    ORGN: Project Ladon                                   */
/*    FILE: wing.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#pragma once

#include <map>
#include <string>
#include <cmath>
#include <nlohmann/json.hpp>
#include "utility.h"

using namespace std;

class Wing
{
public:
    Wing();
    Wing(const string cl, const string cd, const string tail, const float lever, const float area);
    ~Wing();

    // set up functions
    void setCl(const string cl) {cl_table = loadMap(cl);};
    void setCd(const string cd) {cd_table = loadMap(cd);};
    void setTail(const string tail) {aoa_table = loadMap(tail);};
    void setLever(const float lever) {lever_arm = lever;};
    void setArea(const float area) {sail_area = area;};

    // calculation functions
    void calculate(const float windAngle, const float windSpeed, const float tail, const float density);
    float getLongitudinalForce() {return long_force;};
    float getTransverseForce() {return trans_force;};
    float getTorque() {return torque;};

private:

    // Config variables
    map<float, float> cl_table;
    map<float, float> cd_table;
    map<float, float> aoa_table;
    float sail_area = 0;
    float lever_arm = 0;

    // State variables
    float long_force = 0;
    float trans_force = 0;
    float torque = 0;
};
