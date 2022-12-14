# ROS_ADS_Beckhoff

## Overview

A ROS package for communication with Beckhoff's ADS devices

**Keywords:** ROS, ADS, Beckhoff ADS

### License

The source code is released under a [MIT License](LICENSE).

## Installation

#### Dependencies

- [Robot Operating System (ROS)](http://wiki.ros.org) (middleware for robotics),
- [libyaml-cpp-dev](https://github.com/jbeder/yaml-cpp)


#### Building

	cd ros_workspace/src
	git clone -b main --recursive https://github.com/Capacites/ros_ADS_Beckhoff.git
	cd ..
	catkin_make

## Node


#### Architecture

![Archi](Archi.png "Achitecture")


The package provides a C++ node: `rosrun ros_ads_node ros_ads_node`

While continuously updating the state of the ADS device's variables, the node may publish two types of messages:
- The variables' state (on timer or on event depending on the configuration file)
- The node state (indicating wether published data should be considered valid or not, and a debug code)

## Published Topics 

* **`state`** ([ros_ads_msgs/State])

  Publishes the node state in a custom message, the state will be published on timer and on event:
	
		std_msgs/Header header            # header info
		bool state                        # Data validity
		uint8 error 					            # error number

* **`report_timer`** ([ros_ads_msgs/ADS])

  Publishes in a custom message the IO values on timer callback for IOs configured as such in the configuration file:
	
    std_msgs/Header header            # header info
    string[] varNames                 # variables name list
    string[] varTypes                 # variables type list
    float64[] varValues               # variables value list

* **`report_event`** ([ros_ads_msgs/ADS])

  Publishes in a custom message the IO values on event callback for IOs configured as such in the configuration file:
	
		std_msgs/Header header            # header info
    string[] varNames                 # variables name list
    string[] varTypes                 # variables type list
    float64[] varValues               # variables value list

## Subscribed Topics 

* **`command`** ([ros_ads_msgs/ADS])

  Subscribes to commands, write the received values on the associated outputs of the device:
	
    std_msgs/Header header            # header info
    string[] varNames                 # variables name list
    string[] varTypes                 # variables type list
    float64[] varValues               # variables value list

## Parameters

- `name`
    - Type: string
    - Description: Name of the device, must be the same in the configuration file
- `config_file`
    - Type: string
    - Description: File containing the configuration to be loaded

## Configuration file

A configuration file example can be found in `ros_ads_node/config` as `configuration.yaml`

## Supported types

ros_ads only supports the following ADS variable types :

  - BOOL
  - BYTE
  - USINT
  - SINT
  - WORD
  - UINT
  - DINT
  - LINT
  - REAL
  - LREAL
  - DATE

## Utilities

ros_ads_msgs provides a `decode` function to use on a [ros_ads_msgs/ADS] message and returning `std::map<std::string, variant_t>`.
`variant_t` is defined as `std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm>`.

## Setup your ADS route

![route setup help](Add_PLC_route.PNG "route setup help")

## Limitation

You can connect different devices with different remote ip to same ip on PC

You can't create multiple route between same PC IP adesse and same IP device. 
If do this, you could have connection issue/
if you still have connection issue check if an other process use this route (pyads,.... ) .
