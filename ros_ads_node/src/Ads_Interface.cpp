#include "Ads_Interface.h"

using namespace std;

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

RosAds_Interface::RosAds_Interface()
{
}

int RosAds_Interface::checkVariable(string varName){
  int varType = -1;
    map<string,pair<int, string>>::iterator it;
     it= m_VariableMapping.find(varName);
  if(it != m_VariableMapping.end())
  {
    varType = it->second.first;
  }
  return varType;
}

bool RosAds_Interface::adsWriteValue(string name, variant_t value){

  int varType = checkVariable(name);
  bool dataCorrect = true;
  bool bresult = true;
  //Sac à break;

 
  //La variable n'existe pas
  if( m_VariableMapping.find(name) == m_VariableMapping.end()){
    //ROS_ERROR("Variable name not correct");
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

      }
      }
    }
    catch(AdsException e)
    {

    }
    m_MemMutex.unlock();
    m_ComMutex.unlock();
  }
  else
  {
    factory(name);
  }

  if(!dataCorrect) {
    //ROS_ERROR("Data size not correct");
    bresult =  false;
  }
  return bresult;
}

RosAds_Interface::variant_t RosAds_Interface::adsReadValue(string name)
{
  variant_t result;

  if(m_RouteMapping.find(name) != m_RouteMapping.end())
  {
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
                  factory(name);
              }
              }
              catch(...)
              {
                  Readstate();
              }
      }
      m_MemMutex.unlock();
      m_ComMutex.unlock();
  }
  return result;
}

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

vector<RosAds_Interface::variant_t> RosAds_Interface::adsReadVariables(vector<string> varNames)
{
  vector<variant_t> result;

  for (auto& name: varNames)
  {
    result.push_back(adsReadValue(name));
  }

  return result;
}

int RosAds_Interface::initRoute()
{
  m_AmsNetIdremoteNetId= new AmsNetId(m_remoteNetId);
  char remoteIpV4[m_remoteIpV4.length()+1];
  strcpy(remoteIpV4, m_remoteIpV4.c_str());
  //ROS_ERROR_STREAM(m_AmsNetIdremoteNetId);
  m_route = new AdsDevice(m_remoteIpV4,*m_AmsNetIdremoteNetId, AMSPORT_R0_PLC_TC3);
  return 0;
}

int RosAds_Interface::Readstate()
{
    auto result = false;
    AdsDeviceState test;
    auto temp_state = m_device_state;
    m_ComMutex.lock();
    try {
        test = m_route->GetState();
        result = (test.ads == ADSSTATE_RUN);
        m_device_state = result;
        m_ads_state = (int)test.ads;
    } catch (...) {
    }

    if (!result)
    {
        m_device_state = false;
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
    if(result && !temp_state)
    {
        for(auto &[name, alias]: m_VariableMapping)
        {
            factory(name);
        }
    }
    m_ComMutex.unlock();
    return (int)test.ads;
}

bool RosAds_Interface::factory(string  varName)
{
    bool result = false;
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

    }
    m_MemMutex.unlock();
    return result;
}

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

bool  RosAds_Interface::addVariable(string name)
{
    bool result = false;
    auto it = m_VariableMapping.find(name);
    if(it != m_VariableMapping.end())
    {
        m_variables_map[name] = pair<string, variant_t>(m_VariableMapping[name].second, variant_t());
        result = true;
    }
    return result;
}

void RosAds_Interface::updateMemory()
{
    for(auto &[name, pair]: m_variables_map)
    {
        pair.second = adsReadValue(name);
    }
}
