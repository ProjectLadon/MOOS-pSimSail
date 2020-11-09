/************************************************************/
/*    NAME: Pierce Nichols                                  */
/*    ORGN: Project Ladon                                   */
/*    FILE: utility.h                                       */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#pragma once

#include <map>
#include <string>
#include <cmath>
#include <nlohmann/json.hpp>

using namespace std;

map<float, float> loadMap(string input);
float tableLookup(map<float, float> table, float target);
bool loadBoundedValue(string input, float min, float max, float &val);

/// @brief Normalizes input to the range (0.0-360.0) inclusive
/// @param float in -- The number to normalize
/// @return The normalized value
inline double normalizeHeadingDeg(double in)
{
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

inline double deg2rad_f (double deg) {return (deg*M_PI/180);}
inline double rad2deg_f (double rad) {return (rad*180/M_PI);}
