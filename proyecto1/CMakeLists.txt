cmake_minimum_required(VERSION 2.8.3)
project(proyecto1)


find_package(catkin REQUIRED COMPONENTS roscpp geometry_msgs)


catkin_package()

include_directories(include ${catkin_INCLUDE_DIRS})


add_executable(poseUsr poseUsr.cpp)
target_link_libraries(poseUsr ${catkin_LIBRARIES})



