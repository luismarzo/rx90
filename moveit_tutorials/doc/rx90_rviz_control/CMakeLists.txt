#add_definitions(-lpthread)


#find_package(catkin REQUIRED COMPONENTS
 # roscpp
 # rospy
 # std_msgs
 # genmsg
#  gazebo_msgs
#)
#find_package(Threads REQUIRED)
#generate_messages(DEPENDENCIES std_msgs gazebo_msgs)

#catkin_package(

#)


#include_directories(
#   ${catkin_INCLUDE_DIRS}
#)


file(GLOB_RECURSE SRC_FILES "src/*.cpp" "include/*.h" "include/*.inl" )



#find_package(gazebo REQUIRED)
#include_directories(${GAZEBO_INCLUDE_DIRS})
#link_directories(${GAZEBO_LIBRARY_DIRS})
#list(APPEND CMAKE_CXX_FLAGS "${GAZEBO_CXX_FLAGS}")

#find_package(gazebo REQUIRED)





#add_executable(rx90_rviz_control ${SRC_FILES})
#target_link_libraries(rx90_rviz_control ${catkin_LIBRARIES} ${Boost_LIBRARIES})
#install(TARGETS rx90_rviz_control DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
#include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/doc ${PROJECT_SOURCE_DIR}/include)
#target_link_libraries(rx90_rviz_control pthread serial)
#target_link_libraries(rx90_rviz_control ${catkin_LIBRARIES})
#target_link_libraries(rx90_rviz_control ${CMAKE_THREAD_LIBS_INIT})
#target_link_libraries(rx90_rviz_control ${GAZEBO_LIBRARIES})
#add_dependencies(rx90_rviz_control rx90_rviz_control_generate_messages_cpp)

add_executable(rx90_rviz_control src/test.cpp src/Rx90.cpp)
include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(rx90_rviz_control ${catkin_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(rx90_rviz_control pthread serial)
target_link_libraries(rx90_rviz_control ${catkin_LIBRARIES})
target_link_libraries(rx90_rviz_control ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(rx90_rviz_control ${GAZEBO_LIBRARIES})
add_dependencies(rx90_rviz_control rx90_rviz_control_generate_messages_cpp)
install(TARGETS rx90_rviz_control DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
