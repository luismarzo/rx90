


file(GLOB_RECURSE SRC_FILES "src/*.cpp" "include/*.h" "include/*.inl" )





link_directories(${GAZEBO_LIBRARY_DIRS})
include_directories(${Boost_INCLUDE_DIR} ${catkin_INCLUDE_DIRS} ${GAZEBO_INCLUDE_DIRS})



add_executable(rx90_rviz_control src/test.cpp src/Rx90.cpp)
include_directories(PUBLIC ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(rx90_rviz_control ${catkin_LIBRARIES} ${Boost_LIBRARIES})
target_link_libraries(rx90_rviz_control pthread serial)
target_link_libraries(rx90_rviz_control ${catkin_LIBRARIES})
target_link_libraries(rx90_rviz_control ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(rx90_rviz_control ${GAZEBO_LIBRARIES})
link_directories(${GAZEBO_LIBRARY_DIRS})
add_dependencies(rx90_rviz_control rx90_rviz_control_generate_messages_cpp)
install(TARGETS rx90_rviz_control DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION})
