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

# Googletest CMake example begin
# ==============================
# Download and unpack googletest at configure time
configure_file(tests/CMakeLists.txt.gtest googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
  RESULT_VARIABLE result
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
  message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker
# settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines
# the gtest and gtest_main targets.
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)
# END Googletest block
# ====================================================

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


find_program(CTAGS ctags)
if (CTAGS)
	FIND_FILE(MAKE_CTAGS make_ctags.sh ../..)
    ADD_CUSTOM_TARGET(simsail_ctags ALL
        ${MAKE_CTAGS} .
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS pSimSail
    )
endif(CTAGS)

#=============================================================================
# Test Setup
#=============================================================================
