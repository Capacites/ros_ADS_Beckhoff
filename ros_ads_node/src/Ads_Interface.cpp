#include "Ads_Interface.h"

using namespace std;

/**
 * @brief RosAds_Interface simple consctructor
 */
RosAds_Interface::RosAds_Interface()
{
}

RosAds_Interface::~RosAds_Interface()
{
  if(m_route)
  {
    delete m_route;
  }
  if(m_AmsNetIdremoteNetId)
  {
    delete m_AmsNetIdremoteNetId;
  }

  for(map<string,IAdsVariable*>::iterator it = m_RouteMapping.begin(); it != m_RouteMapping.end(); ++it)
  {
     delete it->second;
  }
}

/**
 * @brief adsWriteValue writes on a single variable
 * @param name name of the variable to write on
 * @param value value of the variable to write on
 * @return true if the writing succeded
 * @return false otherwise
 */
bool RosAds_Interface::adsWriteValue(string name, variant_t value){

  int varType = checkVariableType(name);
  bool dataCorrect = true;
  bool bresult = true;
  auto no_issue = true;
  if( m_VariableMapping.find(name) == m_VariableMapping.end()){
    dataCorrect =  false;
    bresult =  false;
  }
  else if (m_VariableMapping[name].first == varType && m_device_state)
  {
    m_ComMutex.lock();
    m_MemMutex.lock();
    try
    {
      switch(m_VariableMapping[name].first)
      {
      case BOOL:
      {
          *m_RouteMapping[name] = get<bool>(value);
          break;
      }
      case UINT8_T:
      {
          *m_RouteMapping[name] = get<uint8_t>(value);
          break;
      }
      case INT8_T:
      {
          *m_RouteMapping[name] = get<int8_t>(value);
          break;
      }
      case UINT16_T:
      {
          *m_RouteMapping[name] = get<uint16_t>(value);
          break;
      }
      case INT16_T:
      {
          *m_RouteMapping[name] = get<int16_t>(value);
          break;
      }
      case UINT32_T:
      {
          *m_RouteMapping[name] = get<uint32_t>(value);
          break;
      }
      case INT32_T:
      {
          *m_RouteMapping[name] = get<int32_t>(value);
          break;
      }
      case INT64_T:
      {
          *m_RouteMapping[name] = get<int64_t>(value);
          break;
      }
      case FLOAT:
      {
          *m_RouteMapping[name] = get<float>(value);
          break;
      }
      case DOUBLE:
      {
          *m_RouteMapping[name] = get<double>(value);
          break;
      }
      case DATE:
      {
          tm temp = get<tm>(value);
          *m_RouteMapping[name] = mktime(&temp);
          break;
      }
      default:
      {
            no_issue = false;
      }
      }
    }
    catch(AdsException e)
    {
        no_issue = false;
    }
    m_MemMutex.unlock();
    m_ComMutex.unlock();
  }
  else
  {
    factory(name);
  }
  if(!no_issue){
      factory(name);
  }

  if(!dataCorrect) {
    bresult =  false;
  }
  return bresult;
}

/**
 * @brief adsReadValue reads a single variable's value
 * @param varName name of the variable to read
 * @return variant_t value of the variable
 */
RosAds_Interface::variant_t RosAds_Interface::adsReadValue(string name)
{
  variant_t result;

  if(m_RouteMapping.find(name) != m_RouteMapping.end())
  {
      auto no_issue = true;
      m_ComMutex.lock();
      m_MemMutex.lock();
      if(m_device_state)
      {
          try
          {
              if(m_RouteMapping[name])
              {
                  m_RouteMapping[name]->ReadValue(&m_temp);

                  switch(m_VariableMapping[name].first)
                  {
                  case BOOL:
                  {
                      result =(bool)m_temp;
                      break;
                  }
                  case UINT8_T:
                  {
                      result = (uint8_t)m_temp;
                      break;
                  }
                  case INT8_T:
                  {
                      result = (int8_t)m_temp;
                      break;
                  }
                  case UINT16_T:
                  {
                      result = (uint16_t)m_temp;
                      break;
                  }
                  case INT16_T:
                  {
                      result = (int16_t)m_temp;
                      break;
                  }
                  case UINT32_T:
                  {
                      result = (uint32_t)m_temp;
                      break;
                  }
                  case INT32_T:
                  {
                      result = (int32_t)m_temp;
                      break;
                  }
                  case INT64_T:
                  {
                      result = (int64_t)m_temp;
                      break;
                  }
                  case FLOAT:
                  {
                      result = (float)m_temp;
                      break;
                  }
                  case DOUBLE:
                  {
                      result = (double)m_temp;
                      break;
                  }
                  case DATE:
                  {
                      result = (uint32_t)m_temp;
                      break;
                  }
                  default:
                  {

                  }
                  }
              }
              else
              {
                  no_issue = false;
              }
              }
              catch(...)
              {
                no_issue = false;
              }
      }
      m_MemMutex.unlock();
      m_ComMutex.unlock();
      if(!no_issue)
      {
          factory(name);
      }
  }
  return result;
}

/**
 * @brief adsReadVariables reads multiple variables' value
 * @param varNames names of the variables to read
 * @return vector<variant_t> values of the variables
 */
vector<RosAds_Interface::variant_t> RosAds_Interface::adsReadVariables(vector<string> varNames)
{
  vector<variant_t> result;

  for (auto& name: varNames)
  {
    result.push_back(adsReadValue(name));
  }

  return result;
}

/**
 * @brief factory (re)-create an IADS variable
 * @param varName the alias of the variable to (re)-create
 * @return true if the creation succeeded
 */
bool RosAds_Interface::factory(string  varName)
{
    bool result = false;
    auto no_issue = true;
    m_MemMutex.lock();
    try {
        string type = m_VariableADS[m_Alias_map[varName]];
        do{
            if(m_RouteMapping[varName])
            {
                delete m_RouteMapping[varName];
            }
          if(type == "BOOL"){
            m_RouteMapping[varName] = new AdsVariable<bool>(*m_route, m_Alias_map[varName]);
            result = true;
            break;
          }
          if(type == "BYTE" || type == "USINT"){
            m_RouteMapping[varName] = new AdsVariable<uint8_t>(*m_route, m_Alias_map[varName]);
            result = true;
            break;
          }
          if(type == "SINT"){
            m_RouteMapping[varName] = new AdsVariable<int8_t>(*m_route, m_Alias_map[varName]);
            result = true;
            break;
          }
          if(type == "WORD" || type == "UINT"){
            m_RouteMapping[varName] = new AdsVariable<uint16_t>(*m_route, m_Alias_map[varName]);
            result = true;
            break;
          }
          if(type == "INT"){
            m_RouteMapping[varName] = new AdsVariable<int16_t>(*m_route, m_Alias_map[varName]);
            result = true;
            break;
          }
          if(type == "DWORD" || type == "UDINT" || type == "DATE" || type == "TIME" || type == "TIME_OF_DAY" || type == "LTIME"){
            m_RouteMapping[varName] = new AdsVariable<uint32_t>(*m_route, m_Alias_map[varName]);
            result = true;
            break;
          }
          if(type == "DINT"){
            m_RouteMapping[varName] = new AdsVariable<int32_t>(*m_route, m_Alias_map[varName]);
            result = true;
            break;
          }
          if(type == "LINT"){
            m_RouteMapping[varName] = new AdsVariable<int64_t>(*m_route, m_Alias_map[varName]);
            result = true;
            break;
          }
          if(type == "REAL"){
            m_RouteMapping[varName] = new AdsVariable<float>(*m_route, m_Alias_map[varName]);
            result = true;
            break;
          }
          if(type == "LREAL"){
            m_RouteMapping[varName] = new AdsVariable<double>(*m_route, m_Alias_map[varName]);
            result = true;
            break;
          }
        }
        while(false);
    } catch (...) {
        no_issue = false;
    }
    m_MemMutex.unlock();

    if(!no_issue){
        m_ads_state = false;
        connectionCheck();
    }

    return result;
}

/**
 * @brief convert_type_from_string gets the type from a string
 * @param type the type as string
 * @return the type as int
 * @return -1 if not a type
 */
int RosAds_Interface::convert_type_from_string(string type)
{
   int result = -1;
   do
   {
       if(type == "BOOL")
       {
           result = BOOL;
           break;
       }
       if(type == "BYTE" || type == "USINT")
       {
           result = UINT8_T;
           break;
       }
       if(type == "SINT")
       {
           result = INT8_T;
           break;
       }
       if(type == "WORD" || type == "UINT")
       {
           result = UINT16_T;
           break;
       }
       if(type == "INT")
       {
           result = INT16_T;
           break;
       }
       if(type == "DWORD" || type == "UDINT")
       {
           result = UINT32_T;
           break;
       }
       if(type == "DINT")
       {
           result = INT32_T;
           break;
       }
       if(type == "LINT")
       {
           result = INT64_T;
           break;
       }
       if(type == "REAL")
       {
           result = FLOAT;
           break;
       }
       if(type == "LREAL")
       {
           result = DOUBLE;
           break;
       }
       if(type == "DATE")
       {
           result = DATE;
           break;
       }
   }
   while(false);
   return result;
}

/**
 * @brief updateMemory update the variables memory
 */
void RosAds_Interface::updateMemory()
{
    for(auto &[name, pair]: m_variables_map)
    {
        pair.second = adsReadValue(name);
    }
}

/**
 * @brief initRoute initialize a connection to the ADS device
 */
void RosAds_Interface::initRoute()
{
  m_AmsNetIdremoteNetId= new AmsNetId(m_remoteNetId);
  m_route = new AdsDevice(m_remoteIpV4.c_str(),*m_AmsNetIdremoteNetId, AMSPORT_R0_PLC_TC3);
}

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
int RosAds_Interface::connectionCheck()
{
    auto result = false;
    AdsDeviceState test;
    auto temp_state = m_device_state;
    m_ComMutex.lock();
    try {
        test = m_route->GetState();
        result = (test.ads == ADSSTATE_RUN);
        m_ads_state = (uint16_t)test.ads;
    } catch (...) {
        m_ads_state = ADSSTATE_INVALID;
    }

    if (!result) //recovery
    {
        if(m_route)
        {
          delete m_route;
        }
        if(m_AmsNetIdremoteNetId)
        {
          delete m_AmsNetIdremoteNetId;
        }
        initRoute();
    }
    m_ComMutex.unlock();

    if(result && !temp_state) //recreate ADSVariables if connexion is re-established
    {
        acquireVariables();
        for(auto &[name, alias]: m_VariableMapping)
        {
            factory(name);
        }
    }

    m_device_state = result;

    return (int)test.ads;
}

/**
 * @brief bindPLcVar creates IADS variables for aliased variables given in configuration file
 * @return true if aliasing succeeded
 * @return false otherwise
 */
bool RosAds_Interface::bindPLcVar()
{
  bool bresult = false;
  YAML::Node config = YAML::LoadFile(m_config_file);
  if (config[m_name])
  {

    //Read each alias with corresponding ADS name
    for(YAML::const_iterator element=config[m_name]["variables"].begin();element!=config[m_name]["variables"].end();++element)
    {
      string adsName = element->first.as<string>();
      string alias = element->second.as<string>();
      //Check if ADS name is part of downloaded PLC ADS list
      if ( m_VariableADS.find(adsName) == m_VariableADS.end() )
      {
          continue;
      }

      string type = m_VariableADS[adsName];
      m_VariableMapping[alias] = pair<int, string>(convert_type_from_string(type), type);
      m_variables_map[alias] = pair<string, variant_t>(type, variant_t());
      m_Alias_map[alias] = adsName;
      factory(alias);
    }
    bresult = true;
  }
  else
  {

  }
    return bresult;
}

/**
 * @brief checkVariableType
 * @param varName the name of the variable to check the type of
 * @return the type of the variable as int
 * @return -1 if variable does not exist
 */
int RosAds_Interface::checkVariableType(string varName){
  int varType = -1;
    map<string,pair<int, string>>::iterator it;
     it= m_VariableMapping.find(varName);
  if(it != m_VariableMapping.end())
  {
    varType = it->second.first;
  }
  return varType;
}
