#ifndef HEADER_H_ADS_INTERFACE
#define HEADER_H_ADS_INTERFACE

#include <yaml-cpp/yaml.h>
#include <cstdlib>
#include "../lib/AdsLib/standalone/AdsDef.h"
#include "../lib/AdsLib/AdsLib.h"
#include "../lib/AdsLib/AdsVariable.h"
#include <time.h>
#include <boost/thread/thread.hpp>
#include <variant>
#include <mutex>

using namespace std;

class RosAds_Interface
{
    enum{
        BOOL,
        UINT8_T,
        INT8_T,
        UINT16_T,
        INT16_T,
        UINT32_T,
        INT32_T,
        INT64_T,
        FLOAT,
        DOUBLE,
        DATE,
    };

public :

  using variant_t = variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm>;

  RosAds_Interface();
  ~RosAds_Interface();

  bool adsWriteValue(string name, variant_t value);
  variant_t adsReadValue(string varName);
  vector<variant_t> adsReadVariables(vector<string> varNames);
  bool factory(string varName);
  int convert_type_from_string(string);

  void setRemoteNetID(string netId){m_remoteNetId = netId;}
  string getRemoteNetID(){return m_remoteNetId;}
  void setRemoteIPV4(string address){m_remoteIpV4 = address;}
  string getRemoteIPV4(){return m_remoteIpV4;}
  void setLocalNetID(string netId){m_localNetId_param = netId;
                                  AdsSetLocalAddress(AmsNetId(m_localNetId_param));}
  string getLocalNetID(){return m_localNetId_param;}
  bool addVariable(string);
  map<string, pair<string, variant_t>> getVariablesMap(){return m_variables_map;}
  void updateMemory();
  void addMonitoredVariable(string);
  void addNotification();

  int initRoute();
  int Readstate();
  bool getState(){return(m_device_state);}
  int getADSState(){return(m_ads_state);}
  bool bindPLcVar();
  int checkVariable(string varName);
  void acquireVariables(){m_VariableADS = m_route->GetDeviceAdsVariables();}
  void setName(string name){m_name = name;}
  void setFile(string config_file){m_config_file = config_file;}

private:

  string m_remoteNetId;
  string m_remoteIpV4;
  string m_localNetId_param;
  double m_temp;
  string m_name;
  string m_config_file;
  int m_ads_state;

  AmsNetId* m_AmsNetIdremoteNetId;
  AdsDevice* m_route;
  bool m_device_state{true};
  bool m_changed{false};


  //Mutex utile pour la communication
  mutex m_ComMutex;
  mutex m_MemMutex;

  map<string,string> m_VariableADS;
  map<string,string> m_Alias_map;
  map<string,pair<int, string>> m_VariableMapping;
  map<string,IAdsVariable*> m_RouteMapping;
  map<string, pair<string, variant_t>> m_variables_map;

  mutex m_VariableMapping_guard;
  mutex m_RouteMapping_guard;
  mutex m_variables_map_guard;
};
#endif
