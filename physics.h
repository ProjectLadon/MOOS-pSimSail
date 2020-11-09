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
#include <string>
#include "wing.h"
#include "foil.h"
#include "utility.h"

using namespace std;

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
    inline const Wing* getForeWing() {return &forewing;};
    inline const Wing* getMizzen() {return &mizzen;};
    inline const Foil* getKeel() {return &keel;};
    inline const Foil* getSkeg() {return &skeg;};

    // read out boat variables
    inline float getLongVelocity() const {return long_velocity;};
    inline float getTransVelocity() const {return trans_velocity;};
    inline float getAngularVelocity() const {return omega;};
    inline float getLeeway() const {return rad2deg_f(theta);};
    inline float getHeading() const {return rad2deg_f(heading);};
    inline float getCourseOverGround() const {return rad2deg_f(heading - theta);};
    inline float getSpeed() const {return(sqrt((long_velocity * long_velocity) + (trans_velocity * trans_velocity)));}

    // Report out
    string buildReport();

private:
    // config variables
    Wing forewing;
    Wing mizzen;
    Foil keel;
    Foil skeg;
    float water_drag;
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
