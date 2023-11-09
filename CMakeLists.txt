cmake_minimum_required(VERSION 3.0)

project(network)

add_executable(network main.cpp)


find_package(Boost COMPONENTS thread system REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
target_link_libraries(network ${Boost_LIBRARIES})


install(TARGETS network RUNTIME DESTINATION bin)
