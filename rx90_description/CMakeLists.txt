cmake_minimum_required(VERSION 2.8.3)
project(rx90_description)

IF(UNIX)
	add_definitions(-std=c++11)
ENDIF()

find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
 # hecatonquiros
)


 #catkin_package(CATKIN_DEPENDS roscpp std_msgs hecatonquiros)
catkin_package(CATKIN_DEPENDS roscpp std_msgs )

#find_package(OpenRAVE)
#if(${OpenRAVE_FOUND})
#	add_definitions(-DHAS_OPENRAVE)
#else(${OpenRAVE_FOUND})
#	MESSAGE(WARNING "OpenRAVE NOT FOUND")
#endif(${OpenRAVE_FOUND})



include_directories(
# include
  ${catkin_INCLUDE_DIRS}
)

#add_executable(rx90_ik "src/rx90_ik.cpp")
#target_link_libraries(rx90_ik ${catkin_LIBRARIES})
#add_dependencies(rx90_ik rx90_ik_generate_messages_cpp)

#if(${OpenRAVE_FOUND})
#	MESSAGE(STATUS "OpenRAVE FOUND")
#	target_include_directories(rx90_ik PUBLIC ${OpenRAVE_INCLUDE_DIRS})
#	target_link_libraries(rx90_ik LINK_PUBLIC ${OpenRAVE_CORE_LIBRARIES})
#	set_target_properties(rx90_ik PROPERTIES COMPILE_FLAGS "${OpenRAVE_CXX_FLAGS}")
#	set_target_properties(rx90_ik PROPERTIES LINK_FLAGS "${OpenRAVE_LINK_FLAGS}")
#else(${OpenRAVE_FOUND})
#	MESSAGE(WARNING "OpenRAVE NOT FOUND")
#endif(${OpenRAVE_FOUND})

#target_include_directories(rx90_ik PUBLIC ${PROJECT_SOURCE_DIR}/include)

#target_include_directories(rx90_ik PUBLIC ${OpenCV_INCLUDE_DIRS})
#target_link_libraries(rx90_ik LINK_PUBLIC ${OpenCV_LIBS})

#target_include_directories(rx90_ik PUBLIC ${PCL_INCLUDE_DIRS})
#target_link_libraries(rx90_ik LINK_PUBLIC ${PCL_LIBRARIES})

#target_include_directories(rx90_ik PUBLIC ${PCL_INCLUDE_DIRS})
#target_link_libraries(rx90_ik LINK_PUBLIC ${PCL_LIBRARIES})

#target_include_directories(rx90_ik PUBLIC ${catkin_INCLUDE_DIRS})
#target_link_libraries(rx90_ik ${catkin_LIBRARIES} hecatonquiros)

#target_include_directories(rx90_ik PUBLIC ${serial_INDLUDE_DIR})
#target_link_libraries(rx90_ik LINK_PUBLIC ${serial_LIBRARIES})

#target_include_directories(rx90_ik PUBLIC "/usr/include/eigen3")



