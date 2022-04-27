#ifndef HEADER_H_ADS_NODE
#define HEADER_H_ADS_NODE

//YAML include
#include <yaml-cpp/yaml.h>

#include "ros/ros.h"
#include "ros/package.h"
#include <ros/callback_queue.h>
#include "Ads_Interface.h"
#include <ros_ads_msgs/ADS.h>
#include <ros_ads_msgs/State.h>
#include <ros_ads_msgs/ADSDecode.h>


//Standard includes
#include <memory>
#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <set>

#include <boost/thread.hpp>

using namespace std::chrono_literals;
using namespace std;

namespace ads_node {

class ADSNode
{

public:
    bool initialize();
    void GetDeviceAdsVariables();
    void timerCallback(int* timer_rate);
    void checkerCallback(int* timer_rate);
    bool SubscriberCallback(const ros_ads_msgs::ADS::ConstPtr& msg);
    void publishTimerCallback(int* timer_rate);

private:

    bool m_publish{false};                /*!< Control to publish event message         */
    bool m_configOK{false};               /*!< Configuration state to the modbus device */

    int m_rate_update;
    int m_rate_state;
    int m_rate_publish;

    map<string, pair<string, double>> m_publish_on_timer;
    map<string, pair<string, double>> m_publish_on_event;
    map<string, pair<string, double>> m_variables;
    mutex m_publish_on_timer_guard;
    mutex m_publish_on_event_guard;
    mutex m_variables_guard;

    pair<string, double> m_checker_temp_value;

    RosAds_Interface m_ADS = RosAds_Interface();
    string m_name;
    string m_config_file;

    ros_ads_msgs::ADS m_on_event_msg;
    ros_ads_msgs::ADS m_on_timer_msg;
    ros_ads_msgs::State m_state_msg;

    ros::Publisher m_state_publisher;
    ros::Publisher m_on_event_publisher;
    ros::Publisher m_on_timer_publisher;
    ros::Subscriber m_subscriber;

    shared_ptr<boost::thread> m_update_thread;
    shared_ptr<boost::thread> m_checker_thread;
    shared_ptr<boost::thread> m_timer_thread;
    shared_ptr<boost::thread> m_subscriber_thread;
};

}

#endif
