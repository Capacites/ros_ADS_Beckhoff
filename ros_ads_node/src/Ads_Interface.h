#include "ros/ros.h"
#include "ros/package.h"
#include <tinyxml.h>
#include <cstdlib>
#include "../lib/AdsLib/standalone/AdsDef.h"
#include "../lib/AdsLib/AdsLib.h"
#include "../lib/AdsLib/AdsVariable.h"
#include <time.h>
#include <boost/thread.hpp>
#include <ros_ads_msgs/ADSReadValue.h>
#include <ros_ads_msgs/ADSReadVariables.h>
#include <ros_ads_msgs/ADSWriteValue.h>

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
  /*std::map<string,AdsVariable<bool>*> BOOLRouteMapping;
  std::map<string,AdsVariable<uint8_t>*> BYTERouteMapping;
  std::map<string,AdsVariable<int8_t>*> SINTRouteMapping;
  std::map<string,AdsVariable<uint16_t>*> UINTRouteMapping;
  std::map<string,AdsVariable<int16_t>*> INTRouteMapping;
  std::map<string,AdsVariable<uint32_t>*> UDINTRouteMapping;
  std::map<string,AdsVariable<int32_t>*> DINTRouteMapping;
  std::map<string,AdsVariable<uint64_t>*> ULONGRouteMapping;
  std::map<string,AdsVariable<int64_t>*> LONGRouteMapping;
  std::map<string,AdsVariable<float>*> FLOATRouteMapping;
  std::map<string,AdsVariable<double>*> DOUBLERouteMapping;*/

  int initRoute();
  bool bindPLcVar();

  string checkVariable(string varName);
public :
  RosAds_server_node();
  ~RosAds_server_node();
  int main(int argc, char **argv);

  bool adsWriteValue(ros_ads_msgs::ADSWriteValue  &req, ros_ads_msgs::ADSWriteValue &res);
  bool adsReadValue(ros_ads_msgs::ADSReadValue  &req, ros_ads_msgs::ADSReadValue &res);
  bool adsReadVariables(ros_ads_msgs::ADSReadVariables  &req, ros_ads_msgs::ADSReadVariables &res);

  bool factory(string varName);




};
