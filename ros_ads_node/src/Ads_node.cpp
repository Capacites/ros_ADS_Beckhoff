#include "Ads_node.h"

using namespace ads_node;

bool ADSNode::SubscriberCallback(ros_ads_msgs::ADS msg)
{
    bool result = true;
    try
    {
        for(auto indx = 0; indx < (int)msg.varNames.size(); indx++)
        {
            result = result && m_ADS.adsWriteValue(msg.varNames[indx], msg.varValues[indx]);
        }
    }
    catch (AdsException e)
    {
        ROS_ERROR_STREAM(e.what());
        result = false;
    }
    return result;
}

void ADSNode::timerCallback()
{
    try
    {
        m_ADS.updateMemory();
        m_variables_map = m_ADS.getVariablesMap();
    }
    catch (...)
    {

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
      nprive.getParam("config_file", m_config_file);
    }
    else {
      ROS_ERROR("Param localNetId unknown");
      return 0;
    }

    if(nprive.hasParam("localNetId"))
    {
      nprive.getParam("localNetId", m_localNetId_param);
      m_ADS.setLocalNetID(m_localNetId_param);
    }
    else
    {
      ROS_ERROR("Param localNetId unknown");
      return 0;
    }

    if(nprive.hasParam("remoteNetId"))
    {
      nprive.getParam("remoteNetId", m_remoteNetId);
      m_ADS.setRemoteNetID(m_remoteNetId);
    }
    else
    {
      ROS_ERROR("Param remoteNetId unknown");
      return 0;
    }

    if(nprive.hasParam("remoteIpV4"))
    {
      nprive.getParam("remoteIpV4", m_remoteIpV4);
      m_ADS.setRemoteNetID(m_remoteIpV4);
    }
    else
    {
      ROS_ERROR("Param remoteIpV4 unknown");
      return 0;
    }

    ROS_ERROR_STREAM("GO FOR Init Route");
    m_ADS.initRoute();
    ROS_ERROR("Init Route done");

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
      ROS_INFO_STREAM(e.what());
      ROS_ERROR("ERROR in mapping alias with ADS");
    }
    return 1;
}

bool ADSNode::main(int argc, char **argv)
{
    auto result = initialize(argc, argv);
    while (ros::ok())
    {

    }
}

int main(int argc, char **argv)
{
    ADSNode *node = new ADSNode();
    return node->main(argc, argv);
}
