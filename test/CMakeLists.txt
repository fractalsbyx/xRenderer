#cmake_minimum_required(VERSION 3.10)
#project(Grid2DViewer)
#
##set(CMAKE_CXX_STANDARD 17)
##set(CMAKE_CXX_STANDARD_REQUIRED ON)
#
## Find SFML (you can adjust version as needed)
##set (CMAKE_FIND_FRAMEWORK NEVER)
##set(SFML_USE_STATIC_LIBS FALSE)
#set(CMAKE_PREFIX_PATH SFML_DIR)
#find_package(SFML 2 COMPONENTS Graphics REQUIRED)
#
#
#add_executable(Grid2DViewer sfml-test.cc)
#
## Link SFML libraries
#target_link_libraries(Grid2DViewer PRIVATE SFML::Graphics)

cmake_minimum_required(VERSION 3.22)
project(CMakeSFMLProject LANGUAGES CXX)

#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

#set(SFML_USE_STATIC_LIBS TRUE)
#set(SFML_STATIC_LIBRARIES ON)
#set(CMAKE_FIND_FRAMEWORK "NEVER")
set(CMAKE_PREFIX_PATH SFML_DIR/cmake)
find_package(SFML 2 COMPONENTS graphics REQUIRED)
add_executable(main main.cc)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE sfml-graphics)