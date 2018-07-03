cmake_minimum_required(VERSION 2.8.3)
project(rx90_serial)

 if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11 -Wl,--no-as-needed)
 endif()

add_definitions(-lpthread)

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
find_package(Threads REQUIRED)

catkin_package(
#  INCLUDE_DIRS include
#  LIBRARIES rx90_serial
#  CATKIN_DEPENDS roscpp rospy std_msgs
#  DEPENDS system_lib
)

###########
## Build ##
###########

## Specify additional locations of header files
## Your package locations should be listed before other locations
include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

# Collect all sources
file(GLOB_RECURSE SRC_FILES "src/*.cpp" "include/*.h" "include/*.inl" )

# Add Executable
#add_executable(${PROJECT_NAME} ${SRC_FILES})
#include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)

#target_link_libraries(${PROJECT_NAME} pthread serial)
#target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

add_executable(rx90_serial_test ${SRC_FILES})
include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(rx90_serial_test pthread serial)
target_link_libraries(rx90_serial_test ${CMAKE_THREAD_LIBS_INIT})


