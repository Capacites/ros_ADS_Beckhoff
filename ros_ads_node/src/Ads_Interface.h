#include "ros/ros.h"
#include "ros/package.h"
#include <tinyxml.h>
#include <cstdlib>
#include "../lib/AdsLib/standalone/AdsDef.h"
#include "../lib/AdsLib/AdsLib.h"
#include "../lib/AdsLib/AdsVariable.h"
#include <time.h>
#include <boost/thread.hpp>
#include <ros_ads_msgs/ADS.h>
#include <ros_ads_msgs/ADSDecode.h>

using namespace std;
class RosAds_server_node
{
private:
  string m_remoteNetId;
  string m_remoteIpV4;
  string m_PLCFileDefinitionPath;
  string m_localNetId_param;

  AmsNetId* m_AmsNetIdremoteNetId;
  AdsDevice *m_route;

  ///Mutex utile pour la communication
  boost::mutex m_ComMutex;

  std::map<string,string> m_VariableADS;

  std::map<string,string> VariableMapping;
  
  std::map<string,IAdsVariable*> RouteMapping;

  int initRoute();
  bool bindPLcVar();

  string checkVariable(string varName);
public :
  RosAds_server_node();
  ~RosAds_server_node();
  int main(int argc, char **argv);

  bool adsWriteValue(ros_ads_msgs::ADS  &req, ros_ads_msgs::ADS &res);
  bool adsReadValue(ros_ads_msgs::ADS  &req, ros_ads_msgs::ADS &res);
  bool adsReadVariables(ros_ads_msgs::ADS  &req, ros_ads_msgs::ADS &res);

  bool factory(string varName);




};
