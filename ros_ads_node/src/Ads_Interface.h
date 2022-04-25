#ifndef HEADER_H_ADS_INTERFACE
#define HEADER_H_ADS_INTERFACE

#include <cstdlib>
#include "../lib/AdsLib/standalone/AdsDef.h"
#include "../lib/AdsLib/AdsLib.h"
#include "../lib/AdsLib/AdsVariable.h"
#include <time.h>
#include <boost/thread.hpp>
#include <variant>
#include <yaml-cpp/yaml.h>

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
  RosAds_Interface();
  ~RosAds_Interface();

  bool adsWriteValue(std::string name, std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm> value);
  std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm> adsReadValue(std::string varName);
  std::vector<std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm>> adsReadVariables(std::vector<std::string> varNames);
  bool factory(string varName);
  int convert_type_from_string(std::string);

private:

  std::map<std::string, std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm>> m_variables_map;
  string m_remoteNetId;
  string m_remoteIpV4;
  string m_PLCFileDefinitionPath;
  string m_localNetId_param;

  double m_temp;

  AmsNetId* m_AmsNetIdremoteNetId;
  AdsDevice *m_route;

  ///Mutex utile pour la communication
  boost::mutex m_ComMutex;

  std::map<string,string> m_VariableADS;

  std::map<string,std::pair<int, std::string>> m_VariableMapping;

  std::map<string,IAdsVariable*> m_RouteMapping;

  int initRoute();
  bool bindPLcVar(std::string file, std::string name);
  int checkVariable(string varName);
};
#endif
