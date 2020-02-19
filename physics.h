/************************************************************/
/*    NAME: Pierce Nichols                                  */
/*    ORGN: Project Ladon                                   */
/*    FILE: physics.h                                       */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#pragma once

#include <cmath>
#include <memory>
#include <nlohmann/json.hpp>
#include "wing.h"
#include "foil.h"
#include "utility.h"

class BoatModel
{
public:
    BoatModel() {};
    ~BoatModel() {};

    // set up function
    bool readConfEntry(const string key, const string value);

    // calculate
    void calculate(const float windAngle, const float windSpeed, const float foretail, const float mizzentail, const float dt);

    // read out subsections
    const Wing& getForeWing() {return forewing;};
    const Wing& getMizzen() {return mizzen;};
    const Foil& getKeel() {return keel;};
    const Foil& getSkeg() {return skeg;};

    // read out boat variables
    float getLongVelocity() {return long_velocity;};
    float getTransVelocity() {return trans_velocity;};
    float getAngularVelocity() {return omega;};
    float getLeeway() {return rad2deg_f(theta);};
    float getHeading() {return rad2deg_f(heading);};
    float getCourseOverGround() {return rad2rad2deg_f(heading - theta);};

private:
    // config variables
    Wing forewing;
    Wing mizzen;
    Foil keel;
    Foil skeg;
    float mass;
    float inertia_moment;
    float water_density;
    float air_density;

    // state variables
    float long_velocity = 0;
    float trans_velocity = 0;
    float omega = 0;
    float theta = 0;
    float heading = 0;

};
