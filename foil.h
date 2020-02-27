/************************************************************/
/*    NAME: Pierce Nichols                                  */
/*    ORGN: Project Ladon                                   */
/*    FILE: foil.h                                          */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#pragma once

#include <string>
#include <cmath>
#include "utility.h"

using namespace std;

class Foil {
public:
    Foil() {};
    Foil(const float area, const float lever) : foil_area(area), lever_arm(lever) {};
    ~Foil() {};

    // config functions
    void setArea(const float area) {foil_area = area;};
    void setLever(const float lever) {lever_arm = lever;};

    // calculation functions
    void calculate(const float theta, const float speed, const float density);
    inline float getLongitudinalForce() const {return long_force;};
    inline float getTransverseForce() const {return trans_force;};
    inline float getTorque() const {return torque;};

private:
    // config variables
    float foil_area = 0;
    float lever_arm = 0;

    // state variables
    float long_force = 0;
    float trans_force = 0;
    float torque = 0;
};
