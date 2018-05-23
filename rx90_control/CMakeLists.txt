cmake_minimum_required(VERSION 2.8.3)
project(rx90_control)

## Compile as C++11, supported in ROS Kinetic and newer
# add_compile_options(-std=c++11)

## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
)


catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES rx90_control
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
include  ${catkin_INCLUDE_DIRS}
)

find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

find_package(gazebo REQUIRED)

add_library(hola_mundo SHARED src/hola_mundo.cpp)
target_link_libraries(hola_mundo ${GAZEBO_LIBRARIES})

add_library(rx90_pub SHARED src/rx90_pub.cpp)
target_link_libraries(rx90_pub ${GAZEBO_LIBRARIES})

