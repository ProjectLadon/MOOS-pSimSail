#--------------------------------------------------------
# The CMakeLists.txt for:                       pSimSail
# Author(s):                              
#--------------------------------------------------------

SET(SRC
  SimSail.cpp
  SimSail_Info.cpp
  main.cpp
)

ADD_EXECUTABLE(pSimSail ${SRC})

TARGET_LINK_LIBRARIES(pSimSail
   ${MOOS_LIBRARIES}
   apputil
   mbutil
   m
   pthread)

