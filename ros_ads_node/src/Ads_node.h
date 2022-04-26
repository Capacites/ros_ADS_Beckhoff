#ifndef HEADER_H_ADS_NODE
#define HEADER_H_ADS_NODE

#include "ros/ros.h"
#include "ros/package.h"
#include <ros/callback_queue.h>
#include "Ads_Interface.h"
#include <ros_ads_msgs/ADS.h>
#include <ros_ads_msgs/ADSDecode.h>

//YAML include
#include <yaml-cpp/yaml.h>

//Standard includes
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <set>

using namespace std::chrono_literals;
using namespace std;

namespace ads_node {

class ADSNode
{
public:
    bool initialize(int argc, char **argv);
    bool main(int argc, char **argv);
    void GetDeviceAdsVariables();
    void timerCallback();
    void checkerCallback();
    bool SubscriberCallback(ros_ads_msgs::ADS msg);

private:
    string m_localNetId_param;
    string m_remoteNetId;
    string m_remoteIpV4;
    RosAds_Interface m_ADS = RosAds_Interface();
    string m_name;
    string m_config_file;

    ros::CallbackQueue m_subscriber_queue;
    ros::CallbackQueue m_publisher_queue;
    ros::CallbackQueue m_update_queue;
    ros::CallbackQueue m_checker_queue;
    ros::CallbackQueue m_state_queue;

    ros::Publisher m_state_publisher;
    ros::Publisher m_on_event_publisher;
    ros::Publisher m_on_timer_publisher;
    ros::Subscriber m_subscriber;

    map<string, pair<string,  RosAds_Interface::variant_t>> m_variables_map;
    map<string, pair<string,  RosAds_Interface::variant_t>> m_variables_map_temp;
};

}

#endif
