#include "ros/ros.h"
#include "ros/package.h"
#include "beckhoff_plc_control/ADSWriteValue.h"
#include "beckhoff_plc_control/ADSReadValue.h"
#include "beckhoff_plc_control/ADSReadVariables.h"
#include <tinyxml.h>
#include <cstdlib>
#include "../lib/AdsLib/AdsLib.h"
#include "../lib/AdsLib/AdsVariable.h"
#include <time.h>

#include <boost/thread.hpp>
using namespace std;
class RosAds_server_node
{
private:
  string m_remoteNetId;
  string m_remoteIpV4;

  ros::ServiceServer m_writingValueService;
  ros::ServiceServer m_readingValueService;
  ros::ServiceServer m_readingVariablesService;

  AmsNetId* m_AmsNetIdremoteNetId;
  AdsDevice *m_route;

  ///Mutex utile pour la communication
  boost::mutex m_ComMutex;

  int initRoute();
public :
  RosAds_server_node(){};
  int main(int argc, char **argv);

  bool adsWriteValue(beckhoff_plc_control::ADSWriteValue::Request  &req, beckhoff_plc_control::ADSWriteValue::Response &res);
  bool adsReadValue(beckhoff_plc_control::ADSReadValue::Request  &req, beckhoff_plc_control::ADSReadValue::Response &res);
  bool adsReadVariables(beckhoff_plc_control::ADSReadVariables::Request  &req, beckhoff_plc_control::ADSReadVariables::Response &res);




};
