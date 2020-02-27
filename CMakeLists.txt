#--------------------------------------------------------
# The CMakeLists.txt for:                       pSimSail
# Author(s):
#--------------------------------------------------------

SET(SRC
  SimSail.cpp
  SimSail_Info.cpp
  main.cpp
  physics.cpp
  wing.cpp
  foil.cpp
  utility.cpp
)

#=============================================================================
# FINDING MOOSGeodesy' HEADERS AND LIBRARIES...
#=============================================================================
find_package(MOOSGeodesy)
include_directories(${MOOSGeodesy_INCLUDE_DIRS})
link_directories(${MOOSGeodesy_LIBRARY_PATH})

message("+++++++++++++++++++++++++++++++++++++++++")
message("MOOSGeodesy_INCLUDE_DIRS:" ${MOOSGeodesy_INCLUDE_DIRS})
message("MOOSGeodesy_LIB_PATH:"     ${MOOSGeodesy_LIBRARY_PATH})
message("+++++++++++++++++++++++++++++++++++++++++")

INCLUDE_DIRECTORIES(${MOOS_INCLUDE_DIRS} ${MOOSGeodesy_INCLUDE_DIRS})

ADD_EXECUTABLE(pSimSail ${SRC})

TARGET_LINK_LIBRARIES(pSimSail
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   proj
   MOOSGeodesy
   pthread)
