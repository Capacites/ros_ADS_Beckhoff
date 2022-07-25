#ifndef HEADER_H_ADS_INTERFACE
#define HEADER_H_ADS_INTERFACE

#include <yaml-cpp/yaml.h>
#include <cstdlib>
#include "../lib/ADS/AdsLib/standalone/AdsDef.h"
#include "../lib/ADS/AdsLib/AdsLib.h"
#include "../lib/ADS/AdsLib/AdsVariable.h"
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

    /**
     * @brief RosAds_Interface simple consctructor
     */
    RosAds_Interface();

    ~RosAds_Interface();

    /**
     * @brief adsWriteValue writes on a single variable
     * @param name name of the variable to write on
     * @param value value of the variable to write on
     * @return true if the writing succeded
     * @return false otherwise
     */
    bool adsWriteValue(string name, variant_t value);

    /**
     * @brief adsReadValue reads a single variable's value
     * @param varName name of the variable to read
     * @return variant_t value of the variable
     */
    variant_t adsReadValue(string varName);

    /**
     * @brief adsReadVariables reads multiple variables' value
     * @param varNames names of the variables to read
     * @return vector<variant_t> values of the variables
     */
    vector<variant_t> adsReadVariables(vector<string> varNames);

    /**
     * @brief factory (re)-create an IADS variable
     * @param varName the alias of the variable to (re)-create
     * @return true if the creation succeeded
     */
    bool factory(string varName);

    /**
     * @brief convert_type_from_string gets the type from a string
     * @param type the type as string
     * @return the type as int
     * @return -1 if not a type
     */
    int convert_type_from_string(string type);

    /**
     * @brief setRemoteNetID set the remote NetID
     * @param netId netID to set as string of type: xxx.xxx.xxx.xxx.xxx.xxx
     */
    void setRemoteNetID(string netId){m_remoteNetId = netId;}

    /**
     * @brief getRemoteNetID get the remote NetID
     * @return netId netID parameter
     */
    string getRemoteNetID(){return m_remoteNetId;}

    /**
     * @brief setRemoteIPV4 set the remoteIPV4
     * @param address IPV4 to set as string of type: xxx.xxx.xxx.xxx
     */
    void setRemoteIPV4(string address){m_remoteIpV4 = address;}

    /**
     * @brief getRemoteIPV4 get the remote IPV4
     * @return IPV4 IPV4 parameter
     */
    string getRemoteIPV4(){return m_remoteIpV4;}

    /**
     * @brief setLocalNetID set the local NetID
     * @param netId netID to set as string of type: xxx.xxx.xxx.xxx.xxx.xxx
     */
    void setLocalNetID(string netId){m_localNetId_param = netId;
                                  AdsSetLocalAddress(AmsNetId(m_localNetId_param));}

    /**
     * @brief getLocalNetID get the local NetID
     * @return netId netID parameter
     */
    string getLocalNetID(){return m_localNetId_param;}

    /**
     * @brief getVariablesMap
     * @return a copy of the variables memory
     */
    map<string, pair<string, variant_t>> getVariablesMap(){return m_variables_map;}

    /**
     * @brief updateMemory update the variables memory
     */
    void updateMemory();

    /**
     * @brief initRoute initialize a connection to the ADS device
     */
    void initRoute();

    /**
     * @brief connectionCheck
     *
     * Tries to get ADS state
     * If the ADS is not running setsm_device state to false, to true otherwise
     * If the ADS is not running or connection failed, tries to establish a new connection
     * If connection was down and is up again, recreates all IADS variables (get the new handles)
     *
     * @return ads state
     */
    int connectionCheck();

    /**
     * @brief getState
     * @return true if data published are valid
     * @return false otherwise
     */
    bool getState(){return(m_device_state);}

    /**
     * @brief getADSState
     * @return return current ADS state
     */
    int getADSState(){return(m_ads_state);}

    /**
     * @brief bindPLcVar creates IADS variables for aliased variables given in configuration file
     * @return true if aliasing succeeded
     * @return false otherwise
     */
    bool bindPLcVar();

    /**
     * @brief checkVariableType
     * @param varName the name of the variable to check the type of
     * @return the type of the variable as int
     * @return -1 if variable does not exist
     */
    int checkVariableType(string varName);

    /**
     * @brief acquireVariables get all ADS variables from the device
     */
    void acquireVariables(){m_VariableADS = m_route->GetDeviceAdsVariables();}

    /**
     * @brief setName set the name of the device for configuration
     * @param name the name of the device
     */
    void setName(string name){m_name = name;}

    /**
     * @brief setFile set the configuration file the device is configured from
     * @param config_file the full path to the YAML configuration file
     */
    void setFile(string config_file){m_config_file = config_file;}


private:

    string m_remoteNetId;                                  /*!< the NetID of the ADS device                                                                                     */
    string m_remoteIpV4;                                   /*!< the IPV4 of the ADS device                                                                                      */
    string m_localNetId_param;                             /*!< the local net ID                                                                                                */
    double m_temp;                                         /*!< a temp value for reading a variable value                                                                       */
    string m_name;                                         /*!< the name of the device for configuration                                                                        */
    string m_config_file;                                  /*!< the configuration file to use                                                                                   */
    int m_ads_state;                                       /*!< the last known state of ADS                                                                                     */

    AmsNetId* m_AmsNetIdremoteNetId;                       /*!< the NetID structure of the ADS device                                                                           */
    AdsDevice* m_route;                                    /*!< the ADS device route                                                                                            */
    bool m_device_state{true};                             /*!< the last known validity state of the values                                                                     */


    //Mutex utile pour la communication
    mutex m_ComMutex;                                      /*!< Communication mutex                                                                                             */
    mutex m_MemMutex;                                      /*!< memory mutex                                                                                                    */

    map<string,string> m_VariableADS;                      /*!< a map with ADS name as key and the variable type as value */
    map<string,string> m_Alias_map;                        /*!< a map with alias name as key and ADS names as value */
    map<string,pair<int, string>> m_VariableMapping;       /*!< a map with alias name as key and both representation of it's type a value                                       */
    map<string,IAdsVariable*> m_RouteMapping;              /*!< a map with alias name as key and the IADS variable as value                                                     */
    map<string, pair<string, variant_t>> m_variables_map;  /*!< a map with alias name as key and a pair with the type of the variable as string and it's value in it's own type */
    };
#endif
