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

    /**
     * @brief ADSNode::initialize initialize the parameters of the node and it's variables
     * @return 1 if the initialization failed
     * @return 0 otherwise
     */
    bool initialize();

    /**
     * @brief ADSNode::update update internal variable values memory
     *
     * Starts by verifying and updating connection state
     *
     * @param timer_rate rate to update at
     */
    void update(int timer_rate);

    /**
     * @brief ADSNode::publishState publish state periodically
     * @param timer_rate rate to publish at
     */
    void publishState(int timer_rate);

    /**
     * @brief ADSNode::publishOnEvent publish variables if an event occured
     * @param timer_rate rate to verify if an event occured at
     */
    void publishOnEvent(int timer_rate);

    /**
     * @brief ADSNode::publishOnTimer publish variables periodically
     * @param timer_rate rate to publish at
     */
    void publishOnTimer(int timer_rate);

    /**
     * @brief ADSNode::Subscriber send the ADS device the received commands
     * @param msg the command message
     * @return true if the command was successfully sent
     */
    bool Subscriber(const ros_ads_msgs::ADS::ConstPtr& msg);

private:

    bool m_publish{false};                                     /*!< Control to publish event message                          */
    bool m_configOK{false};                                    /*!< Configuration state to the modbus device                  */

    int m_rate_update;                                         /*!< update and event detection rate                           */
    int m_rate_state;                                          /*!< state publication rate                                    */
    int m_rate_publish;                                        /*!< periodical publish rate                                   */

    map<string, pair<string, double>> m_publish_on_timer;      /*!< caracteristics of variables to publish periodically       */
    map<string, pair<string, double>> m_publish_on_event;      /*!< caracteristics of variables to publish on event           */
    map<string, pair<string, double>> m_variables;             /*!< caracteristics of declared variables                      */
    mutex m_publish_on_timer_guard;                            /*!< m_publish_on_timer map guard                              */
    mutex m_publish_on_event_guard;                            /*!< m_publish_on_event map guard                              */
    mutex m_variables_guard;                                   /*!< m_variables map guard                                     */

    pair<string, double> m_checker_temp_value;                 /*!< temporary value for event detection                       */

    RosAds_Interface m_ADS = RosAds_Interface();               /*!< ADS device interface                                      */
    string m_name;                                             /*!< name of the device in the configuration file              */
    string m_config_file;                                      /*!< configuration file with the description of the ADS device */

    ros_ads_msgs::ADS m_on_event_msg;                          /*!< message to publish on event                               */
    ros_ads_msgs::ADS m_on_timer_msg;                          /*!< message to publish on timer                               */
    ros_ads_msgs::State m_state_msg;                           /*!< state message                                             */

    ros::Publisher m_state_publisher;                          /*!< state publisher                                           */
    ros::Publisher m_on_event_publisher;                       /*!< on event publisher                                        */
    ros::Publisher m_on_timer_publisher;                       /*!< periodical publisher                                      */
    ros::Subscriber m_subscriber;                              /*!< command subscriber                                        */

    shared_ptr<boost::thread> m_update_thread;                 /*!< update thread                                             */
    shared_ptr<boost::thread> m_checker_thread;                /*!< event detection thread                                    */
    shared_ptr<boost::thread> m_timer_thread;                  /*!< on timer publisher thread                                 */
    shared_ptr<boost::thread> m_state_thread;                  /*!< state publisher thread                                    */
};

}

#endif
