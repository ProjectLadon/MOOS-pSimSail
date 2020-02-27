/************************************************************/
/*    NAME: Pierce Nichols                                  */
/*    ORGN: Project Ladon                                   */
/*    FILE: utility.cpp                                     */
/*    DATE: December 29th, 1963                             */
/************************************************************/

#include <map>
#include <string>
#include <cmath>
#include <nlohmann/json.hpp>
#include "utility.h"

using namespace std;
using namespace nlohmann;   // For convenience with JSON

map<float, float> loadMap(string input) {
    json in = json::parse(input);
    map<float, float> result;
    if (in.is_array()) {
        for (auto &a: in) {
            if (a.is_array() && a[0].is_number() && a[1].is_number()) {
                result.emplace(a[0].get<float>(), a[1].get<float>());
            }
        }
    }
    return result;
}

bool loadBoundedValue(string input, float min, float max, float &val) {
    float in = stof(input);
    if (in < min) return false;
    if (in > max) return false;
    val = in;
    return true;
}

float tableLookup(map<float, float> table, float target) {
    auto top = table.cbegin();
    auto bot = table.cbegin();
    float result;
    // Note that this loop depends on the fact that map keys are sorted
    for (auto it = table.cbegin(); it != table.cend(); ++it) {
        if (it->first <= target) bot = it;
        if (it->first >= target) {
            top = it;
            break;
        }
    }
    if (top->first == bot->first) {
        result = top->second;
    } else {
        result = interpolate(target, bot->first, bot->second, top->first, top->second);
    }
    return result;
}
