cmake_minimum_required(VERSION 2.8.3)
project(rx90_control)


find_package(catkin REQUIRED COMPONENTS
  roscpp
  rospy
  std_msgs
  genmsg
  gazebo_msgs
  message_generation
)

add_service_files(
  FILES
  Set_activation.srv
  Set_position.srv
)

generate_messages(DEPENDENCIES std_msgs gazebo_msgs)

catkin_package(

)




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

add_library(rx90_publisher SHARED src/rx90_publisher.cpp)
target_link_libraries(rx90_publisher ${GAZEBO_LIBRARIES})

add_library(rx90_pub SHARED src/rx90_pub.cpp)
target_link_libraries(rx90_pub ${GAZEBO_LIBRARIES})

add_executable(main_publisher "src/main_publisher.cpp")
target_link_libraries(main_publisher ${catkin_LIBRARIES})
add_dependencies(main_publisher rx90_control_generate_messages_cpp)

add_executable(main_catch "src/main_catch.cpp")
target_link_libraries(main_catch ${catkin_LIBRARIES})
add_dependencies(main_catch rx90_control_generate_messages_cpp)



#add_executable(positioner_plugin src/positioner_plugin.cpp)
add_library(gripper_plugin SHARED src/gripper_plugin.cpp)
target_link_libraries(gripper_plugin ${GAZEBO_LIBRARIES})


add_executable(Set_activation_client src/clients/Set_activation_client.cpp)
target_link_libraries(Set_activation_client ${catkin_LIBRARIES})
add_dependencies(Set_activation_client rx90_control_gencpp)

add_executable(Set_position_client src/clients/Set_position_client.cpp)
target_link_libraries(Set_position_client ${catkin_LIBRARIES})
add_dependencies(Set_position_client rx90_control_gencpp)

#add_executable(collision_plugin src/collision_plugin.cpp)
add_library(contact_plugin SHARED src/contact_plugin.cpp)
target_link_libraries(contact_plugin ${GAZEBO_LIBRARIES})
