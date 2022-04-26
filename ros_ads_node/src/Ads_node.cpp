#include "Ads_node.h"

#include <iostream>

using namespace ads_node;

bool ADSNode::SubscriberCallback(const ros_ads_msgs::ADS::ConstPtr &msg)
{
    bool result = true;
    auto command = ros_ads_msgs::decode(msg);

    try {
        for (auto &[name, value]: command)
        {
            m_ADS.adsWriteValue(name, value);
        }
    }
    catch (...)
    {

    }

    return result;
}

void ADSNode::timerCallback(int* timer_rate)
{
    ros::Rate loop_rate(*timer_rate);
    while(ros::ok())
    {
        try
        {
            m_ADS.updateMemory();
            auto variables_map = m_ADS.getVariablesMap();
            for(auto &[name, pair]: variables_map)
            {
                m_variables[name].first = pair.first;
                switch (pair.second.index())
                {
                case ros_ads_msgs::BOOL:
                {
                    m_variables[name].second = static_cast<double>(get<bool>(pair.second));
                }
                case ros_ads_msgs::UINT8_T:
                {
                    m_variables[name].second = static_cast<double>(get<uint8_t>(pair.second));
                }
                case ros_ads_msgs::INT8_T:
                {
                    m_variables[name].second = static_cast<double>(get<int8_t>(pair.second));
                }
                case ros_ads_msgs::UINT16_T:
                {
                    m_variables[name].second = static_cast<double>(get<uint16_t>(pair.second));
                }
                case ros_ads_msgs::INT16_T:
                {
                    m_variables[name].second = static_cast<double>(get<int16_t>(pair.second));
                }
                case ros_ads_msgs::UINT32_T:
                {
                    m_variables[name].second = static_cast<double>(get<uint32_t>(pair.second));
                }
                case ros_ads_msgs::INT32_T:
                {
                    m_variables[name].second = static_cast<double>(get<int32_t>(pair.second));
                }
                case ros_ads_msgs::INT64_T:
                {
                    m_variables[name].second = static_cast<double>(get<int64_t>(pair.second));
                }
                case ros_ads_msgs::FLOAT:
                {
                    m_variables[name].second = static_cast<double>(get<float>(pair.second));
                }
                case ros_ads_msgs::DOUBLE:
                {
                    m_variables[name].second = get<double>(pair.second);
                }
                case ros_ads_msgs::DATE:
                {
                    tm temp = get<tm>(pair.second);
                    m_variables[name].second = static_cast<double>(mktime(&temp));
                }
                default:
                {

                }
                }
            }
        }
        catch (...)
        {

        }
        loop_rate.sleep();
    }
}

void ADSNode::checkerCallback(int* timer_rate)
{
    ros::Rate loop_rate(*timer_rate);
    while(ros::ok())
    {
        m_publish = false;
        for(auto &[key, pair_] : m_publish_on_event)
        {
            m_checker_temp_value = m_variables[key];
            if(pair_ != m_checker_temp_value) // A change has occured
            {
                m_publish_on_event[key] = m_checker_temp_value; // Update value

                if(!m_publish)
                {
                    m_publish = true;
                }
            }
        }
        if (m_publish) // Publish if a change occured
        {
            m_on_event_msg.header.stamp.setNow(ros::Time());
            m_on_event_msg.varNames = vector<string>();
            m_on_event_msg.varTypes = vector<string>();
            m_on_event_msg.varValues = vector<double>();
            for (auto &[key, pair] : m_publish_on_event)
            {
                m_on_event_msg.varNames.push_back(key);
                m_on_event_msg.varTypes.push_back(pair.first);
                m_on_event_msg.varValues.push_back(pair.second);
            }
            if(m_on_event_msg.varNames.size() !=0)
            {
                m_on_event_publisher.publish(m_on_event_msg);
            }

        }
        loop_rate.sleep();
    }

}

void ADSNode::publishTimerCallback(int* timer_rate)
{

    ros::Rate loop_rate(*timer_rate);
    while(ros::ok())
    {
        m_on_timer_msg.header.stamp.setNow(ros::Time());
        m_on_timer_msg.varNames = vector<string>();
        m_on_timer_msg.varTypes = vector<string>();
        m_on_timer_msg.varValues = vector<double>();

        for(auto &[name, pair]: m_variables)
        {
            m_on_timer_msg.varNames.push_back(name);
            m_on_timer_msg.varTypes.push_back(pair.first);
            m_on_timer_msg.varValues.push_back(pair.second);
        }

        if(m_on_timer_msg.varNames.size() != 0)
        {
            m_on_timer_publisher.publish(m_on_timer_msg);
        }
        loop_rate.sleep();
    }
}

bool ADSNode::initialize(int argc, char **argv)
{
    ros::init(argc, argv, "ros_ads_node");
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
        try {
            nprive.getParam("config_file", m_config_file);
            YAML::Node config = YAML::LoadFile(m_config_file);
            if(config[m_name])
            {
                m_ADS.setLocalNetID(config[m_name]["localNetID"].as<string>());
                m_ADS.setRemoteNetID(config[m_name]["remoteNetID"].as<string>());
                m_ADS.setRemoteNetID(config[m_name]["remoteIP"].as<string>());
                m_rate_update = config[m_name]["refresh_rate"].as<int>();
                m_rate_publish = config[m_name]["publish_rate"].as<int>();
                m_rate_state = config[m_name]["state_rate"].as<int>();
            }
        } catch (...) {
            ROS_ERROR("Invalid configuration file");
        }

    }
    else {
      ROS_ERROR("Error with configuration file");
      return 0;
    }

    //try
    {
        ROS_INFO_STREAM("GO FOR Init Route");
        m_ADS.initRoute();
        ROS_INFO("Init Route done");
    }
    //catch(...)
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
      m_ADS.bindPLcVar(m_config_file, m_name);
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
        m_subscriber = n.subscribe<ros_ads_msgs::ADS>("chatter", 1, boost::bind(&ADSNode::SubscriberCallback, this, _1));
    }
    catch (...)
    {
        ROS_ERROR("ERROR in creating subscriber");
        return 0;
    }


    return 1;
}

bool ADSNode::main(int argc, char **argv)
{
    bool result = initialize(argc, argv);
    if(result)
    {
        m_timer_thread = new boost::thread(&ADSNode::publishTimerCallback,this, &m_rate_publish);
        m_update_thread = new boost::thread(&ADSNode::timerCallback,this, &m_rate_update);
        m_checker_thread = new boost::thread(&ADSNode::checkerCallback,this, &m_rate_update);
    }
    while (ros::ok())
    {
        if(result)
        {

        }
    }

    return 0;
}

int main(int argc, char **argv)
{
    ADSNode *node = new ADSNode();
    return node->main(argc, argv);
}
