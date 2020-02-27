# MOOS-pSimSail
This is a simulator for wing schooners. It takes the sail set positions and outputs an ongoing simulated position and heading. In the future, it will support multiple sail and hull models.

## Dependencies
* MOOS-IvP
* libMOOSGeodesy
* libgeometry
* https://github.com/Tencent/rapidjson/ -- provides JSON parse/deparse

## Configuration Parameters
### Foresail
	* foresail_cl -- A table of the coefficient of lift of the foresail. It is a JSON array of pairs. The first element of each pair is the AoA in degrees and the second is the coefficient of lift at that AoA.
	* foresail_cd -- A table like foresail_cl for the foresail coefficient of drag.
	* foresail_tail -- A table like foresail_cl, except the first entry in each pair is the tail setting and the second is the angle of attack. All units are in degrees.
	* foresail_area -- Area of the foresail in square meters.
	* foresail_cg_distance -- Distance of the foresail pivot from the center of gravity, in meters forward of the CG.
### Mizzen
	* mizzen_cl -- Like foresail_cl, for the mizzen.
	* mizzen_cd -- Like foresail_cd, for the mizzen.
	* mizzen_tail -- Like foresail_tail, for the mizzen.
	* mizzen_area -- Area of the mizzen in square meters.
	* mizzen_cg_distance -- Distance of the mizzen pivot from the center of gravity, in meters aft of the CG.
### Hull
	* water_drag -- Underwater coefficient of drag times forward projected area, in meters squared.
	* inertia_moment -- Moment of inertia around the center of gravity.
	* mass -- Mass of the vessel, in kilograms.
	* keel_area -- Area of keel, in square meters.
	* keel_cg_distance -- Keel lever arm, in meters forward of the CG.
	* skeg_area -- Area of skeg, in square meters.
	* skeg_cg_distance -- Skeg lever arm, in meters aft of the CG.
### Environment
	* air_density -- In kilograms per cubic meter.
	* water_density -- In kilograms per cubic meter.
	* wind_direction -- The direction the wind is coming from, in degrees clockwise from north
	* wind_speed -- The velocity of the wind, in m/s
	* origin_lat -- Latitude of the zero point of the local coordinate system, in decimal degrees.
	* origin_lon -- Longitude of the zero point of the local coordinate system, in decimal degrees.
	* start_x -- Starting position, in meters north of the origin.
	* start_y -- Starting position, in meters east of the origin.
### Incoming Variables
	* foresail_tail_cmd -- Foresail tail command in degrees
	* mizzen_tail_cmd -- Mizzen tail command in degrees
### Outgoing Variables
	* output_lat_msg
	* output_lon_msg
	* foresail_hdg_msg
	* mizzen_hdg_msg
	* boat_hdg_msg
	* speed_over_gnd_msg
	* course_over_gnd_msg
