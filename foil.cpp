/************************************************************/
/*    NAME: Pierce Nichols                                  */
/*    ORGN: Project Ladon                                   */
/*    FILE: foil.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <string>
#include <cmath>
#include "utility.h"
#include "foil.h"

using namespace std;

void Foil::calculate(const float theta, const float speed, const float density)
{
    float cl = 2 * M_PI * theta;
    float cd = cl * sin(theta);
    float q = 0.5 * density * speed * speed;
    long_force = -(speed/fabs(speed)) * (cd * q * foil_area);
    trans_force = -(speed/fabs(speed)) * (cl * q * foil_area);
    torque = -(trans_force * lever_arm);
}
