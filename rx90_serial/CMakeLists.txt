cmake_minimum_required(VERSION 2.8.3)
project(rx90_serial)

 if(CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=c++11 -Wl,--no-as-needed)
 endif()

add_definitions(-lpthread)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  genmsg
  gazebo_msgs
)
find_package(Threads REQUIRED)
generate_messages(DEPENDENCIES std_msgs gazebo_msgs)

catkin_package(

)


include_directories(
   ${catkin_INCLUDE_DIRS}
)


file(GLOB_RECURSE SRC_FILES "src/*.cpp" "include/*.h" "include/*.inl" )



find_package(gazebo REQUIRED)
include_directories(${GAZEBO_INCLUDE_DIRS})
link_directories(${GAZEBO_LIBRARY_DIRS})
list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

find_package(gazebo REQUIRED)

# Collect all sources


# Add Executable
#add_executable(${PROJECT_NAME} ${SRC_FILES})
#include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)

#target_link_libraries(${PROJECT_NAME} pthread serial)
#target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

add_executable(rx90_serial_test ${SRC_FILES})
include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(rx90_serial_test pthread serial)
target_link_libraries(rx90_serial_test ${catkin_LIBRARIES})
target_link_libraries(rx90_serial_test ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(rx90_serial_test ${GAZEBO_LIBRARIES})
add_dependencies(rx90_serial_test rx90_serial_generate_messages_cpp)


