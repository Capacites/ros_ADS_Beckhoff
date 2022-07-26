cmake_minimum_required(VERSION 3.0.2)
project(ros_ads_node)

## Compile as C++11, supported in ROS Kinetic and newer

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Threads REQUIRED)
set(CMAKE_BUILD_TYPE debug)

add_subdirectory(lib/ADS/AdsLib)
## Find catkin macros and libraries
## if COMPONENTS list like find_package(catkin REQUIRED COMPONENTS xyz)
## is used, also find other catkin packages
find_package(catkin REQUIRED COMPONENTS
  cmake_modules
  roscpp
  roslib
  std_msgs
  message_generation
  ros_ads_msgs
)
find_package(TinyXML REQUIRED)
find_library(yaml-cpp REQUIRED)

catkin_package(
  LIBRARIES 
  CATKIN_DEPENDS roscpp std_msgs roslib
#  DEPENDS system_lib
)



include_directories(
  include
  ${catkin_INCLUDE_DIRS}
  ${TinyXML_INCLUDE_DIRS}
)



add_executable(ros_ads_node src/Ads_Interface.cpp src/Ads_node.cpp)
add_dependencies(ros_ads_node ${catkin_EXPORTED_TARGETS} )
add_dependencies(ros_ads_node  ads)
set_property(TARGET ros_ads_node PROPERTY CXX_STANDARD 17)
set_property(TARGET ros_ads_node PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(ros_ads_node ${catkin_LIBRARIES} ${TinyXML_LIBRARIES} yaml-cpp ads  -lpthread )

## Mark executables for installation
## See http://docs.ros.org/melodic/api/catkin/html/howto/format1/building_executables.html
# install(TARGETS ${PROJECT_NAME}_node
#   RUNTIME DESTINATION ${CATKIN_PACKAGE_BIN_DESTINATION}
# )

install(TARGETS ros_ads_node
  ARCHIVE DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  LIBRARY DESTINATION ${CATKIN_PACKAGE_LIB_DESTINATION}
  RUNTIME DESTINATION ${CATKIN_GLOBAL_BIN_DESTINATION}
)

## Mark other files for installation (e.g. launch and bag files, etc.)
 install(
   DIRECTORY launch/
   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/launch
   PATTERN ".svn" EXCLUDE)
 install(
   DIRECTORY config/
   DESTINATION ${CATKIN_PACKAGE_SHARE_DESTINATION}/config
   PATTERN ".svn" EXCLUDE)
