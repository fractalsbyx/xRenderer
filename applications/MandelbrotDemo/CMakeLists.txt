##
#  CMake script for the Renderer application
##

cmake_minimum_required(VERSION 3.22)

include(${CMAKE_SOURCE_DIR}/../../cmake/setup_application.cmake)
include(${CMAKE_SOURCE_DIR}/../../cmake/xRendererConfig.cmake)

project(myapp CXX)

# Set location of files
include_directories(${CMAKE_SOURCE_DIR}/../../include)
include_directories(SYSTEM ${CMAKE_SOURCE_DIR}/../../vendor)
include_directories(${CMAKE_SOURCE_DIR}/../../src)
include_directories(${CMAKE_SOURCE_DIR})

find_package(SFML 2 COMPONENTS graphics REQUIRED)

# Set the location of the main.cc file
set(TARGET_SRC "${CMAKE_SOURCE_DIR}/main.cc")

# Set targets & link libraries for the build type
if(${X_RENDERER_BUILD_DEBUG} STREQUAL "ON")
  add_executable(main_debug ${TARGET_SRC})
  set_property(TARGET main_debug PROPERTY OUTPUT_NAME main-debug)
  target_compile_options(main_debug PRIVATE ${X_RENDERER_CXX_FLAGS_DEBUG})
  target_link_options(main_debug PRIVATE ${X_RENDERER_LINKER_FLAGS_DEBUG})
  target_compile_options(main_debug PRIVATE ${X_RENDERER_WARNING_FLAGS})
  target_compile_features(main_debug PRIVATE ${X_RENDERER_CXX_STANDARD})
  target_link_libraries(main_debug PRIVATE sfml-graphics)
  set(_build "DEBUG")
  #target_link_libraries(main_debug ${CMAKE_SOURCE_DIR}/../../libxrenderer-debug.a)
endif()

if(${X_RENDERER_BUILD_RELEASE} STREQUAL "ON")
  add_executable(main_release ${TARGET_SRC})
  set_property(TARGET main_release PROPERTY OUTPUT_NAME main)
  target_compile_options(main_release PRIVATE ${X_RENDERER_CXX_FLAGS_RELEASE})
  target_link_options(main_release PRIVATE ${X_RENDERER_LINKER_FLAGS_RELEASE})
  target_compile_options(main_release PRIVATE ${X_RENDERER_WARNING_FLAGS})
  target_compile_features(main_release PRIVATE ${X_RENDERER_CXX_STANDARD})
  target_link_libraries(main_release PRIVATE sfml-graphics)
  set(_build "RELEASE")
  #target_link_libraries(main_release ${CMAKE_SOURCE_DIR}/../../libxrenderer-release.a)
endif()