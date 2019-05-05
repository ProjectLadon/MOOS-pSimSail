# MOOS-pSimSail
This is a simulator for wing schooners. It takes the sail set positions and outputs an ongoing simulated position and heading. In the future, it will support multiple sail and hull models.

## Dependencies
* MOOS-IvP
* libMOOSGeodesy
* libgeometry
* https://github.com/Tencent/rapidjson/ -- provides JSON parse/deparse

## Configuration Parameters
* wingsail -- JSON object configuring one sail. A typical wing schooner will have two. Each sail object must conform to the following schema:
```
{
	"$schema": "http://json-schema.org/schema#",
	"id": "SimSail_WingSail",
	"type": "object",
	"properties": {
        "tail_variable": {"type": "string"}
        "area": {"type": "number"},
        "lever_arm": {
            "type": "object",
            "properties": {
                "x": {"type": "number"},
                "y": {"type": "number"},
                "z": {"type": "number"}
            },
            "required": ["x", "y"]
        },
        "Cl": {
            "type": "array",
            "items": {
                "type": "object",
                "properties": {
                    "AOA": {"type": "number"},
                    "Cl": {"type": "number"}
                },
                "required": ["AOA", "Cl"]
            }
        },
        "Cd": {
            "type": "array",
            "items": {
                "type": "object",
                "properties": {
                    "AOA": {"type": "number"},
                    "Cd": {"type": "number"}
                },
                "required": ["AOA", "Cd"]
            }
        },
        "tail_map": {
            "type": "array",
            "items": {
                "type": "object",
                "properties": {
                    "tail": {"type": "number"},
                    "AOA": {"type": "number"}
                },
                "required": ["tail", "AOA"]
            }
        }
    }
    "required": ["area", "Cl", "Cd", "tail_map"]
}
```
All quantities in MKS units and angles in degrees. Lever arms are in meters forward and port of the center of gravity.

* mass -- The total mass of the vehicle, in kilograms.
* moi -- The vehicle moment of inertia around the center of gravity, in kg-m^2.
* damping -- The rotational damping coefficient, in N-m-s
* wind_direction -- The direction the wind is coming from, in degrees clockwise from north
* wind_speed -- The velocity of the wind, in m/s
* start_latitude -- Latitude of the zero point of the local coordinate system, in decimal degrees.
* start_longitude -- Longitude of the zero point of the local coordinate system, in decimal degrees.

## Incoming Variables
This modules shall subscribe to the variables defined in the wingsail configuration objects.

## Outgoing variables
* SIM_Latitude -- A DOUBLE containing the current latitude
* SIM_Longitude
* SIM_X
* SIM_Y
* SIM_Heading
* SIM_Speed
