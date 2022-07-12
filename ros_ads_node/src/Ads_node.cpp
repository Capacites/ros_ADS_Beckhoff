#include "Ads_node.h"

#include <iostream>
#include <unistd.h>

using namespace ads_node;

/**
 * @brief ADSNode::initialize initialize the parameters of the node and it's variables
 * @return 1 if the initialization failed
 * @return 0 otherwise
 */
bool ADSNode::initialize()
{
    ros::NodeHandle n;
    ros::NodeHandle nprive("~");

    if(nprive.hasParam("name"))
    {
      nprive.getParam("name", m_name);
    }
    else
    {
      ROS_ERROR("Param name unknown");
      return 0;
    }

    if(nprive.hasParam("config_file"))
    {
        try { //get parameters from YAML file
            nprive.getParam("config_file", m_config_file);
            YAML::Node config = YAML::LoadFile(m_config_file);
            if(config[m_name])
            {
                m_ADS.setLocalNetID(config[m_name]["localNetID"].as<string>());
                m_ADS.setRemoteNetID(config[m_name]["remoteNetID"].as<string>());
                m_ADS.setRemoteIPV4(config[m_name]["remoteIP"].as<string>());
                m_rate_update = config[m_name]["refresh_rate"].as<int>();
                m_rate_publish = config[m_name]["publish_rate"].as<int>();
                m_rate_state = config[m_name]["state_rate"].as<int>();

                if(config[m_name]["publish_on_timer"].size() != 0)
                {
                    auto onTimer = config[m_name]["publish_on_timer"].as<vector<string>>();
                    for(auto & var: onTimer)
                    {
                        m_publish_on_timer_guard.lock();
                        m_publish_on_timer[var] = pair<string, double>();
                        m_publish_on_timer_guard.unlock();
                        m_variables_guard.lock();
                        m_variables[var] = pair<string, double>();
                        m_variables_guard.unlock();
                    }
                }

                if(config[m_name]["publish_on_event"].size() != 0)
                {
                    auto onEvent = config[m_name]["publish_on_event"].as<vector<string>>();
                    for(auto & var: onEvent)
                    {
                        m_publish_on_event_guard.lock();
                        m_publish_on_event[var] = pair<string, double>();
                        m_publish_on_event_guard.unlock();
                        m_variables_guard.lock();
                        m_variables[var] = pair<string, double>();
                        m_variables_guard.unlock();
                    }
                }
            }
        } catch (...)
        {
            ROS_ERROR("Invalid configuration file");
        }

    }
    else {
      ROS_ERROR("Error with configuration file");
      return 0;
    }

    try
    {
        ROS_INFO_STREAM("GO FOR Init Route");
        m_ADS.initRoute();
        ROS_INFO("Init Route done");
    }
    catch(...)
    {
        ROS_ERROR("ERROR while connecting with ADS");
        return 0;
    }

    try
    {
      ROS_INFO("Acquiring ADS variables");
      m_ADS.acquireVariables();
      ROS_INFO("ADS variables acquired");

      ROS_INFO("Aliasing ADS variables");
      m_ADS.setName(m_name);
      m_ADS.setFile(m_config_file);
      m_ADS.bindPLcVar();
      ROS_INFO("Ready to communicate with the remote PLC via ADS.");
    }
    catch(AdsException e)
    {
      ROS_ERROR_STREAM(e.what());
      ROS_ERROR("ERROR in mapping alias with ADS");
      return 0;
    }

    try
    {
        m_subscriber = n.subscribe<ros_ads_msgs::ADS>("command", 1, boost::bind(&ADSNode::Subscriber, this, _1));
        m_on_event_publisher = n.advertise<ros_ads_msgs::ADS>("report_event", 10);
        m_on_timer_publisher = n.advertise<ros_ads_msgs::ADS>("report_timer", 10);
        m_state_publisher = n.advertise<ros_ads_msgs::State>("state", 10);
    }
    catch (...)
    {
        ROS_ERROR("ERROR in creating subscriber");
        return 0;
    }

    m_timer_thread = make_shared<boost::thread>(&ADSNode::publishOnTimer,this, m_rate_publish);
    m_update_thread = make_shared<boost::thread>(&ADSNode::update,this, m_rate_update);
    m_checker_thread = make_shared<boost::thread>(&ADSNode::publishOnEvent,this, m_rate_update);
    m_state_thread = make_shared<boost::thread>(&ADSNode::publishState,this, m_rate_state);

    return 1;
}

/**
 * @brief ADSNode::update update internal variable values memory
 *
 * Starts by verifying and updating connection state
 *
 * @param timer_rate rate to update at
 */
void ADSNode::update(int timer_rate)
{
    auto loop_rate_update = ros::Rate(timer_rate);
    while(ros::ok())
    {
        m_variables_guard.lock();
        try
        {
            m_ADS.connectionCheck();
            if(m_ADS.getState())
            {
                m_ADS.updateMemory();
                auto variables_map = m_ADS.getVariablesMap();
                for(auto &[name, pair]: variables_map)
                { //cast as double for the message
                    m_variables[name].first = pair.first;
                    switch (pair.second.index())
                    {
                    case ros_ads_msgs::BOOL:
                    {
                        m_variables[name].second = static_cast<double>(get<bool>(pair.second));
                        break;
                    }
                    case ros_ads_msgs::UINT8_T:
                    {
                        m_variables[name].second = static_cast<double>(get<uint8_t>(pair.second));
                        break;
                    }
                    case ros_ads_msgs::INT8_T:
                    {
                        m_variables[name].second = static_cast<double>(get<int8_t>(pair.second));
                        break;
                    }
                    case ros_ads_msgs::UINT16_T:
                    {
                        m_variables[name].second = static_cast<double>(get<uint16_t>(pair.second));
                        break;
                    }
                    case ros_ads_msgs::INT16_T:
                    {
                        m_variables[name].second = static_cast<double>(get<int16_t>(pair.second));
                        break;
                    }
                    case ros_ads_msgs::UINT32_T:
                    {
                        m_variables[name].second = static_cast<double>(get<uint32_t>(pair.second));
                        break;
                    }
                    case ros_ads_msgs::INT32_T:
                    {
                        m_variables[name].second = static_cast<double>(get<int32_t>(pair.second));
                        break;
                    }
                    case ros_ads_msgs::INT64_T:
                    {
                        m_variables[name].second = static_cast<double>(get<int64_t>(pair.second));
                        break;
                    }
                    case ros_ads_msgs::FLOAT:
                    {
                        m_variables[name].second = static_cast<double>(get<float>(pair.second));
                        break;
                    }
                    case ros_ads_msgs::DOUBLE:
                    {
                        m_variables[name].second = static_cast<double>(get<double>(pair.second));
                        break;
                    }
                    case ros_ads_msgs::DATE:
                    {
                        tm temp = get<tm>(pair.second);
                        m_variables[name].second = static_cast<double>(mktime(&temp));
                        break;
                    }
                    default:
                    {

                    }
                    }
                    m_publish_on_timer_guard.lock();
                    if(m_publish_on_timer.find(name) != m_publish_on_timer.end() && m_ADS.getState())
                    {
                        m_publish_on_timer[name] = m_variables[name];
                    }
                    m_publish_on_timer_guard.unlock();
                }
            }
        }
        catch (...)
        {
            ROS_ERROR("fail timer CB");
        }
        m_variables_guard.unlock();
        loop_rate_update.sleep();
    }
}

/**
 * @brief ADSNode::publishState publish state periodically
 * @param timer_rate rate to publish at
 */
void ADSNode::publishState(int timer_rate)
{
    auto loop_rate_state = ros::Rate(timer_rate);
    while(ros::ok())
    {
        m_state_msg.header.stamp = ros::Time::now();
        m_state_msg.state = m_ADS.getState();
        m_state_msg.error = m_ADS.getADSState();
        m_state_publisher.publish(m_state_msg);
        loop_rate_state.sleep();
    }
}

/**
 * @brief ADSNode::publishOnEvent publish variables if an event occured
 * @param timer_rate rate to verify if an event occured at
 */
void ADSNode::publishOnEvent(int timer_rate)
{
    auto loop_rate_check = ros::Rate(timer_rate);
    while(ros::ok())
    {
        try
        {
            if(m_ADS.getState())
            {
                m_publish_on_event_guard.lock();
                auto publish_on_event = m_publish_on_event;
                m_publish_on_event_guard.unlock();
                m_variables_guard.lock();
                auto variables = m_variables;
                m_variables_guard.unlock();

                m_publish = false;
                for(auto &[key, pair_] : publish_on_event)
                {
                    m_checker_temp_value = variables[key];
                    if(pair_ != m_checker_temp_value) // A change has occured
                    {
                        m_publish_on_event_guard.lock();
                        m_publish_on_event[key] = m_checker_temp_value; // Update value
                        m_publish_on_event_guard.unlock();

                        if(!m_publish)
                        {
                            m_publish = true;
                        }
                    }
                }
                if (m_publish) // Publish if a change occured
                {
                    m_on_event_msg.header.stamp = ros::Time::now();
                    m_on_event_msg.varNames = vector<string>();
                    m_on_event_msg.varTypes = vector<string>();
                    m_on_event_msg.varValues = vector<double>();

                    for (auto &[key, pair] : publish_on_event)
                    {
                        m_on_event_msg.varNames.push_back(key);
                        auto type = pair.first;
                        m_on_event_msg.varTypes.push_back(type);
                        auto value = pair.second;
                        m_on_event_msg.varValues.push_back(value);
                    }
                    if(m_on_event_msg.varNames.size() !=0)
                    {
                        m_on_event_publisher.publish(m_on_event_msg);
                    }
                }
            }
        }
        catch(...)
        {
            ROS_ERROR("fail checker CB");
        }
        loop_rate_check.sleep();
    }

}

/**
 * @brief ADSNode::publishOnTimer publish variables periodically
 * @param timer_rate rate to publish at
 */
void ADSNode::publishOnTimer(int timer_rate)
{

    auto loop_rate_pub = ros::Rate(timer_rate);
    while(ros::ok())
    {
        try
        {
            m_on_timer_msg.header.stamp = ros::Time::now();
            m_on_timer_msg.varNames = vector<string>();
            m_on_timer_msg.varTypes = vector<string>();
            m_on_timer_msg.varValues = vector<double>();

            m_publish_on_timer_guard.lock();
            auto publish_on_timer = m_publish_on_timer;
            m_publish_on_timer_guard.unlock();
            
            for(auto &[name, pair]: publish_on_timer)
            {
                m_on_timer_msg.varNames.push_back(name);
                m_on_timer_msg.varTypes.push_back(pair.first);
                m_on_timer_msg.varValues.push_back(pair.second);
            }


            if(m_on_timer_msg.varNames.size() != 0)
            {
                m_on_timer_publisher.publish(m_on_timer_msg);
            }
            loop_rate_pub.sleep();
        }
        catch(...)
        {
            ROS_ERROR("fail pub CB");
        }

    }
}

/**
 * @brief ADSNode::Subscriber send the ADS device the received commands
 * @param msg the command message
 * @return true if the command was successfully sent
 */
bool ADSNode::Subscriber(const ros_ads_msgs::ADS::ConstPtr &msg)
{
    m_state_publisher.publish(m_state_msg);
    bool result = true;
    auto command = ros_ads_msgs::decode(msg);

    try {
        if(m_ADS.getState())
        {
            for (auto &[name, value]: command)
            {
                m_ADS.adsWriteValue(name, value);
            }
        }
    }
    catch (...)
    {
        result = false;
    }

    return result;
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "ros_ads_node");
    ADSNode *node = new ADSNode();

    node->initialize();

    while(ros::ok())
    {
        ros::spinOnce();
    }
    return 0;
}
