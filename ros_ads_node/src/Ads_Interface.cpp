#include "Ads_Interface.h"

using namespace std;

RosAds_Interface::~RosAds_Interface()
{
  m_ComMutex.lock();
  if(m_route)
  {
    delete m_route;
  }
  if(m_AmsNetIdremoteNetId)
  {
    delete m_AmsNetIdremoteNetId;
  }

  for(std::map<string,IAdsVariable*>::iterator it = m_RouteMapping.begin(); it != m_RouteMapping.end(); ++it)
  {
     delete it->second;
  }
}

RosAds_Interface::RosAds_Interface()
{
}

int RosAds_Interface::checkVariable(string varName){
  int varType = -1;
    std::map<string,std::pair<int, std::string>>::iterator it;
     it= m_VariableMapping.find(varName);
  if(it != m_VariableMapping.end())
  {
    varType = it->second.first;
  }
  return varType;
}

bool RosAds_Interface::adsWriteValue(std::string name, std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm> value){


  int varType = checkVariable(name);
  bool dataCorrect = true;
  bool bresult = true;
  //Sac à break;
  m_ComMutex.lock();
 
  //La variable n'existe pas
  if( m_VariableMapping.find(name) == m_VariableMapping.end()){
    //ROS_ERROR("Variable name not correct");
    dataCorrect =  false;
    bresult =  false;
  }
  else if (m_VariableMapping[name].first == varType)
  {
    try
    {
      switch(m_VariableMapping[name].first)
      {
      case BOOL:
      {
          *m_RouteMapping[name] = std::get<bool>(value);
      }
      case UINT8_T:
      {
          *m_RouteMapping[name] = std::get<uint8_t>(value);
      }
      case INT8_T:
      {
          *m_RouteMapping[name] = std::get<int8_t>(value);
      }
      case UINT16_T:
      {
          *m_RouteMapping[name] = std::get<uint16_t>(value);
      }
      case INT16_T:
      {
          *m_RouteMapping[name] = std::get<int16_t>(value);
      }
      case UINT32_T:
      {
          *m_RouteMapping[name] = std::get<uint32_t>(value);
      }
      case INT32_T:
      {
          *m_RouteMapping[name] = std::get<int32_t>(value);
      }
      case INT64_T:
      {
          *m_RouteMapping[name] = std::get<int64_t>(value);
      }
      case FLOAT:
      {
          *m_RouteMapping[name] = std::get<float>(value);
      }
      case DOUBLE:
      {
          *m_RouteMapping[name] = std::get<double>(value);
      }
      case DATE:
      {
          tm temp = std::get<tm>(value);
          *m_RouteMapping[name] = mktime(&temp);
      }
      default:
      {

      }
      }
    }
    catch(AdsException e)
    {

    }
  }
  else
  {
    factory(name);
  }

  m_ComMutex.unlock();
  if(!dataCorrect) {
    //ROS_ERROR("Data size not correct");
    bresult =  false;
  }
  return bresult;
}

std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm> RosAds_Interface::adsReadValue(std::string name)
{
  std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm> result;

  if(m_RouteMapping.find(name) != m_RouteMapping.end())
  {
      try
      {
        m_ComMutex.lock();
        m_RouteMapping[name]->ReadValue(m_temp);
        m_ComMutex.unlock();
        //ROS_INFO("The %s %s equals %f",varType.c_str(),name.c_str(), res.varValues[0]);
        switch(m_VariableMapping[name].first)
        {
        case BOOL:
        {
            result =(bool)m_temp;
        }
        case UINT8_T:
        {
            result = (uint8_t)m_temp;
        }
        case INT8_T:
        {
            result = (int8_t)m_temp;
        }
        case UINT16_T:
        {
            result = (uint16_t)m_temp;
        }
        case INT16_T:
        {
            result = (int16_t)m_temp;
        }
        case UINT32_T:
        {
            result = (uint32_t)m_temp;
        }
        case INT32_T:
        {
            result = (int32_t)m_temp;
        }
        case INT64_T:
        {
            result = (int64_t)m_temp;
        }
        case FLOAT:
        {
            result = (float)m_temp;
        }
        case DOUBLE:
        {
            result = (double)m_temp;
        }
        case DATE:
        {
            result = (uint32_t)m_temp;
        }
        default:
        {

        }
      }
      }
      catch(AdsException e)
      {
        //ROS_ERROR_STREAM(e.what());
        factory(name);
      }
  }
  return result;
}

bool RosAds_Interface::bindPLcVar(std::string file, std::string name)
{
  bool bresult = false;
  YAML::Node config = YAML::LoadFile(file);
  if (config[name])
  {
    //ROS_INFO("Reading your PLC config file ...");

    //Read each alias with corresponding ADS name
    for(YAML::const_iterator element=config[name]["variables"].begin();element!=config[name]["variables"].end();++element)
    {
      string alias = element->first.as<std::string>();
      string adsName = element->second.as<std::string>();

      //Check if ADS name is part of downloaded PLC ADS list
      if ( m_VariableADS.find(adsName) == m_VariableADS.end() )
      {
        //ROS_ERROR("Error on ADS alias: %s -> %s",alias.c_str(),adsName.c_str());
        continue;
      }

      //ROS_INFO("ADS alias found: %s -> %s",alias.c_str(),adsName.c_str());

      std::string type = m_VariableADS[adsName];
      m_VariableMapping[alias] = std::pair<int, std::string>(convert_type_from_string(type), type);
      m_variables_map[alias] = std::pair<std::string, std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm>>(type, std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm>());

      switch(m_VariableMapping[name].first)
      {
      case BOOL:
      {
          m_RouteMapping[alias] = new AdsVariable<bool>(*m_route, adsName);
      }
      case UINT8_T:
      {
          m_RouteMapping[alias] = new AdsVariable<uint8_t>(*m_route, adsName);
      }
      case INT8_T:
      {
          m_RouteMapping[alias] = new AdsVariable<int8_t>(*m_route, adsName);
      }
      case UINT16_T:
      {
          m_RouteMapping[alias] = new AdsVariable<uint16_t>(*m_route, adsName);
      }
      case INT16_T:
      {
          m_RouteMapping[alias] = new AdsVariable<int16_t>(*m_route, adsName);
      }
      case UINT32_T:
      {
          m_RouteMapping[alias] = new AdsVariable<uint32_t>(*m_route, adsName);
      }
      case INT32_T:
      {
          m_RouteMapping[alias] = new AdsVariable<int32_t>(*m_route, adsName);
      }
      case INT64_T:
      {
          m_RouteMapping[alias] = new AdsVariable<int64_t>(*m_route, adsName);
      }
      case FLOAT:
      {
          m_RouteMapping[alias] = new AdsVariable<float>(*m_route, adsName);
      }
      case DOUBLE:
      {
          m_RouteMapping[alias] = new AdsVariable<double>(*m_route, adsName);
      }
      case DATE:
      {
          m_RouteMapping[alias] = new AdsVariable<uint32_t>(*m_route, adsName);
      }
      default:
      {

      }
      }
    }
    bresult = true;
  }
  else
  {
    //ROS_INFO("Failed to load the PLC config file");
  }
    return bresult;
}

std::vector<std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm>> RosAds_Interface::adsReadVariables(std::vector<std::string> varNames)
{
  std::vector<std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm>> result;

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

bool RosAds_Interface::factory(string  varName)
{
    bool result = false;
    try {
        string type = m_VariableADS[varName];
        do{
          if (m_RouteMapping.find(varName) != m_RouteMapping.end())
          {
              delete m_RouteMapping[varName];
          }
          if(type == "BOOL"){
            m_RouteMapping[varName] = new AdsVariable<bool>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "BYTE" || type == "USINT"){
            m_RouteMapping[varName] = new AdsVariable<uint8_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "SINT"){
            m_RouteMapping[varName] = new AdsVariable<int8_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "WORD" || type == "UINT"){
            m_RouteMapping[varName] = new AdsVariable<uint16_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "INT"){
            m_RouteMapping[varName] = new AdsVariable<int16_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "DWORD" || type == "UDINT" || type == "DATE" || type == "TIME" || type == "TIME_OF_DAY" || type == "LTIME"){
            m_RouteMapping[varName] = new AdsVariable<uint32_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "DINT"){
            m_RouteMapping[varName] = new AdsVariable<int32_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "LINT"){
            m_RouteMapping[varName] = new AdsVariable<int64_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "REAL"){
            m_RouteMapping[varName] = new AdsVariable<float>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "LREAL"){
            m_RouteMapping[varName] = new AdsVariable<double>(*m_route,varName);
            result = true;
            break;
          }
        }
        while(false);
    } catch (AdsException e) {
        //ROS_INFO("error factory");
    }
    return result;
}

int RosAds_Interface::convert_type_from_string(std::string type)
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
