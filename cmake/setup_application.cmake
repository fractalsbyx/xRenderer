# General setup for an application

message(STATUS "")
message(STATUS "=========================================================")
message(STATUS "Configuring xRenderer application")
message(STATUS "=========================================================")
message(STATUS "")

# Setup the build type (debug, release, debugrelease)
if("${CMAKE_BUILD_TYPE}" STREQUAL "")
  set(CMAKE_BUILD_TYPE "DebugRelease" 
      CACHE STRING
      "Choose the type of build, options are: Debug, Release and DebugRelease."
      FORCE)
endif()

# Convert build type into the debug and release builds, which may or may 
# not be built.
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR
   "${CMAKE_BUILD_TYPE}" STREQUAL "Debug" OR
   "${CMAKE_BUILD_TYPE}" STREQUAL "DebugRelease" )
  message(STATUS "Setting up xRenderer application for ${CMAKE_BUILD_TYPE} mode.")
else()
  message(FATAL_ERROR
    "CMAKE_BUILD_TYPE must either be 'Release', 'Debug', or 'DebugRelease', but is set to '${CMAKE_BUILD_TYPE}'.")
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug" OR 
   "${CMAKE_BUILD_TYPE}" STREQUAL "DebugRelease")
  set(X_RENDERER_BUILD_DEBUG "ON")
else()
  set(X_RENDERER_BUILD_DEBUG "OFF")
endif()

if("${CMAKE_BUILD_TYPE}" STREQUAL "Release" OR 
   "${CMAKE_BUILD_TYPE}" STREQUAL "DebugRelease")
  set(X_RENDERER_BUILD_RELEASE "ON")
else()
  set(X_RENDERER_BUILD_RELEASE "OFF")
endif()

# Make and ninja build options
if(CMAKE_GENERATOR MATCHES "Ninja")
  set(_make_command "$ ninja")
else()
  set(_make_command " $ make")
endif()

# Add distclean target to clean build
add_custom_target(distclean
  COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target clean
  COMMAND ${CMAKE_COMMAND} -E remove_directory CMakeFiles
  COMMAND ${CMAKE_COMMAND} -E remove
    CMakeCache.txt cmake_install.cmake Makefile
    build.ninja rules.ninja .ninja_deps .ninja_log
  COMMENT "distclean invoked"
  )