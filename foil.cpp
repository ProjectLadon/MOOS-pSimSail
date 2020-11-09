/************************************************************/
/*    NAME: Pierce Nichols                                  */
/*    ORGN: Project Ladon                                   */
/*    FILE: foil.cpp                                        */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <string>
#include <cmath>
#include "ACTable.h"
#include "utility.h"
#include "foil.h"

using namespace std;

void Foil::calculate(const float theta, const float speed, const float density)
{
    float cl = 2 * M_PI * theta;
    float cd = cl * sin(theta);
    float q = 0.5 * density * speed * speed;
    float sign_mult = 1;
    if (fabs(speed) > 0.0001)
    {
        sign_mult = speed/fabs(speed);
    }
    long_force = -sign_mult * (cd * q * foil_area);
    trans_force = -sign_mult * (cl * q * foil_area);
    torque = -(trans_force * lever_arm);
}

string Foil::buildReport()
{
    ACTable state(3);
    state << "F(x), N | F(y), N | Torque, N-m";
    state.addHeaderLines();
    state << to_string(long_force);
    state << to_string(trans_force);
    state << to_string(torque);
    return state.getFormattedString();
}
