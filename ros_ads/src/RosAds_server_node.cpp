#include "RosAds_server_node.h"

using namespace std;

TiXmlDocument doc;

RosAds_server_node::~RosAds_server_node()
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

  for(std::map<string,IAdsVariable*>::iterator it = RouteMapping.begin(); it != RouteMapping.end(); ++it)
  {
     delete it->second;
  }
}

RosAds_server_node::RosAds_server_node()
{
}

string RosAds_server_node::checkVariable(string varName){
  string varType="";
  ;
    std::map<string,string>::iterator it;
     it= VariableMapping.find(varName);
  if(it != VariableMapping.end())
  {
    varType = it->second;
  }
  return varType;
}

bool RosAds_server_node::adsWriteValue(beckhoff_plc_control::ADSWriteValue::Request  &req, beckhoff_plc_control::ADSWriteValue::Response &res){


  string varType = checkVariable(req.varName);
  bool dataCorrect = true;
  bool bresult = true;
  //Sac à break;
  m_ComMutex.lock();
 
  //La variable n'existe pas
  if(varType == ""){
    ROS_ERROR("Variable name not correct");
    dataCorrect =  false;
    bresult =  false;
  }
  else
  {
    try
    {

      if((uint8_t)req.value == req.value)
      {
        try
        {
          if((uint8_t)req.value == 0){
            *BOOLRouteMapping[req.varName] = false;
            ROS_INFO("The %s %s now equals false",varType.c_str(),req.varName.c_str());
          }else{
            *BOOLRouteMapping[req.varName] = true;
            ROS_INFO("The %s %s now equals true",varType.c_str(),req.varName.c_str());
          }
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete BOOLRouteMapping[req.varName];
          BOOLRouteMapping[req.varName] = new AdsVariable<bool>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }


    if(varType == "BYTE" || varType == "USINT")
    {
      if((uint8_t)req.value == req.value)
      {
        try
        {
          *BYTERouteMapping[req.varName] = (uint8_t)req.value;
          ROS_INFO("The %s %s now equals %u",varType.c_str(),req.varName.c_str(),(uint8_t)req.value);
        }
	catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete BYTERouteMapping[req.varName];
          BYTERouteMapping[req.varName] = new AdsVariable<uint8_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }

    if(varType == "SINT")
    {
      if((int8_t)req.value == req.value)
      {
        try
        {
          *SINTRouteMapping[req.varName] = (int8_t)req.value;
          ROS_INFO("The %s %s now equals %d",varType.c_str(),req.varName.c_str(),(int8_t)req.value);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete SINTRouteMapping[req.varName];
          SINTRouteMapping[req.varName] = new AdsVariable<int8_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }

    if(varType == "WORD" || varType == "UINT")
    {
      if((uint16_t)req.value == req.value)
      {
        try
        {
          *UINTRouteMapping[req.varName] = (uint16_t)req.value;
          ROS_INFO("The %s %s now equals %u",varType.c_str(),req.varName.c_str(),(uint16_t)req.value);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete UINTRouteMapping[req.varName];
          UINTRouteMapping[req.varName] = new AdsVariable<uint16_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }

    if(varType == "INT")
    {
      if((int16_t)req.value == req.value)
      {
        try
        {
          *INTRouteMapping[req.varName] = (int16_t)req.value;
         ROS_INFO("The %s %s now equals %d",varType.c_str(),req.varName.c_str(),(int16_t)req.value);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete INTRouteMapping[req.varName];
          INTRouteMapping[req.varName] = new AdsVariable<int16_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }

    if(varType == "DWORD" || varType == "UDINT")
    {
      if((uint32_t)req.value == req.value)
      {
        try
        {
          *UDINTRouteMapping[req.varName] = (uint32_t)req.value;
          ROS_INFO("The %s %s now equals %u",varType.c_str(),req.varName.c_str(),(uint32_t)req.value);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete UDINTRouteMapping[req.varName];
          UDINTRouteMapping[req.varName] = new AdsVariable<uint32_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }


    if(varType == "DINT")
    {
      if((int32_t)req.value == req.value)
      {
        try
        {
          *DINTRouteMapping[req.varName] = (int32_t)req.value;
          ROS_INFO("The %s %s now equals %d",varType.c_str(),req.varName.c_str(),(int32_t)req.value);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete DINTRouteMapping[req.varName];
          DINTRouteMapping[req.varName] = new AdsVariable<int32_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }

    if(varType == "LWORD" || varType == "ULINT")
    {
      if((uint64_t)req.value == req.value)
      {
        try
        {
          *ULONGRouteMapping[req.varName] = (uint64_t)req.value;
          ROS_INFO("The %s %s now equals %lu",varType.c_str(),req.varName.c_str(),(uint64_t)req.value);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete ULONGRouteMapping[req.varName];
          ULONGRouteMapping[req.varName] = new AdsVariable<uint64_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }

    if(varType == "LINT")
    {
      if((int64_t)req.value == req.value)
      {
        try
        {
          *LONGRouteMapping[req.varName] = (int64_t)req.value;
          ROS_INFO("The %s %s now equals %ld",varType.c_str(),req.varName.c_str(),int64_t(req.value));
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete LONGRouteMapping[req.varName];
          LONGRouteMapping[req.varName] = new AdsVariable<int64_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }

    if(varType == "REAL")
    {
      try
      {
        *FLOATRouteMapping[req.varName] = (float)req.value;
        ROS_INFO("The %s %s now equals %f",varType.c_str(),req.varName.c_str(),(float)req.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete FLOATRouteMapping[req.varName];
        FLOATRouteMapping[req.varName] = new AdsVariable<float>(*m_route,req.varName);
      }
      break;
    }

    if(varType == "LREAL")
    {
      try
      {
        *DOUBLERouteMapping[req.varName] = req.value;
        ROS_INFO("The %s %s now equals %lf",varType.c_str(),req.varName.c_str(),req.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete DOUBLERouteMapping[req.varName];
        DOUBLERouteMapping[req.varName] = new AdsVariable<double>(*m_route,req.varName);
      }
      break;
    }

    if(varType == "DATE")
    {
      if((uint32_t)req.value == req.value)
      {
        try
        {
          *UDINTRouteMapping[req.varName] = (uint32_t)req.value;
          ros::Time currentDate((uint32_t)req.value);
          time_t tDate(currentDate.toSec());
          tm tmDate;
          gmtime_r(&tDate,&tmDate);
          ROS_INFO("%s set to : %d/%d/%d %d:%d:%d",
                 req.varName.c_str(),
                 tmDate.tm_mday,
                 tmDate.tm_mon + 1,
                 tmDate.tm_year + 1900,
                 tmDate.tm_hour,
                 tmDate.tm_min,
                 tmDate.tm_sec);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete UDINTRouteMapping[req.varName];
          UDINTRouteMapping[req.varName] = new AdsVariable<uint32_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
      break;

      *RouteMapping[req.varName] = req.value;
      //ROS_INFO("The %s %s now equals true",varType.c_str(),req.varName.c_str());

    }
    catch(AdsException e)
    {

      if((uint32_t)req.value == req.value)
      {
        try
        {
          if(varType == "TIME" || varType == "TIME_OF_DAY")
          {
            *UDINTRouteMapping[req.varName] = (uint32_t)req.value;
          }
          else if(varType == "LTIME")
          {
            *UDINTRouteMapping[req.varName] = (uint32_t)(req.value * 1000000);
          }
          long lTime = (long)(req.value);
          long lMs = lTime % 1000;
          long lSec = (lTime/1000) % 60;
          long lMin = (lTime/60000) % 60;
          long lHeu = (lTime/3600000) % 24;
          ROS_INFO("%s set to %luh%lum%lus%lums",req.varName.c_str(),lHeu,lMin,lSec,lMs);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete UDINTRouteMapping[req.varName];
          UDINTRouteMapping[req.varName] = new AdsVariable<uint32_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
      break;

      ROS_ERROR_STREAM(e.what());
      bresult =  false;

    }
  }
  m_ComMutex.unlock();
  if(!dataCorrect) {
    ROS_ERROR("Data size not correct");
    bresult =  false;
  }
  return bresult;
}

bool RosAds_server_node::adsReadValue(beckhoff_plc_control::ADSReadValue::Request  &req, beckhoff_plc_control::ADSReadValue::Response &res)
{
  bool bresult =  true;;

  string varType = checkVariable(req.varName);
  m_ComMutex.lock();
  
  if(varType == ""){
    ROS_ERROR("Variable name not correct");
    bresult = false;
  }

  m_ComMutex.lock();
  do{
    if(varType == "BOOL")
    {
      try
      {
        res.value = *BOOLRouteMapping[req.varName];
        if(*BOOLRouteMapping[req.varName] == false)
        {
          ROS_INFO("The %s %s equals false",varType.c_str(),req.varName.c_str());
        }
        else
        {
          ROS_INFO("The %s %s equals true",varType.c_str(),req.varName.c_str());
        }
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete BOOLRouteMapping[req.varName];
        BOOLRouteMapping[req.varName] = new AdsVariable<bool>(*m_route,req.varName);
      }
      break;
    }
    if(varType == "BYTE" || varType == "USINT")
    {
      try
      {
        res.value = *BYTERouteMapping[req.varName];
        ROS_INFO("The %s %s equals %u",varType.c_str(),req.varName.c_str(),(uint8_t)res.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete BYTERouteMapping[req.varName];
        BYTERouteMapping[req.varName] = new AdsVariable<uint8_t>(*m_route,req.varName);
      }
 
      break;
    }
    if(varType == "SINT")
    {
      try
      {
        res.value = *SINTRouteMapping[req.varName];
        ROS_INFO("The %s %s equals %d",varType.c_str(),req.varName.c_str(),(int8_t)res.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete SINTRouteMapping[req.varName];
        SINTRouteMapping[req.varName] = new AdsVariable<int8_t>(*m_route,req.varName);
      }
 
      break;
    }
    if(varType == "WORD" || varType == "UINT")
    {
      try
      {
        res.value = *UINTRouteMapping[req.varName];
        ROS_INFO("The %s %s equals %u",varType.c_str(),req.varName.c_str(),(uint16_t)res.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete UINTRouteMapping[req.varName];
        UINTRouteMapping[req.varName] = new AdsVariable<uint16_t>(*m_route,req.varName);
      }
 
      break;
    }
    if(varType == "INT")
    {
      try
      {
        res.value = *INTRouteMapping[req.varName];
        ROS_INFO("The %s %s equals %d",varType.c_str(),req.varName.c_str(),(int16_t)res.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete INTRouteMapping[req.varName];
        INTRouteMapping[req.varName] = new AdsVariable<int16_t>(*m_route,req.varName);
      }
 
      break;
    }

    if(varType == "DWORD" || varType == "UDINT")
    {
      try
      {
        res.value = *UDINTRouteMapping[req.varName];
        ROS_INFO("The %s %s equals %u",varType.c_str(),req.varName.c_str(),(uint32_t)res.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete UDINTRouteMapping[req.varName];
        UDINTRouteMapping[req.varName] = new AdsVariable<uint32_t>(*m_route,req.varName);
      }
 
      break;
    }

    if(varType == "DINT")
    {
      try
      {
        res.value = *DINTRouteMapping[req.varName];
        ROS_INFO("The %s %s equals %d",varType.c_str(),req.varName.c_str(),(int32_t)res.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete DINTRouteMapping[req.varName];
        DINTRouteMapping[req.varName] = new AdsVariable<int32_t>(*m_route,req.varName);
      }
 
      break;
    }

    if(varType == "LWORD" || varType == "ULINT")
    {
      try
      {
        res.value = *ULONGRouteMapping[req.varName];
        ROS_INFO("The %s %s equals %lu",varType.c_str(),req.varName.c_str(),(uint64_t)res.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete ULONGRouteMapping[req.varName];
        ULONGRouteMapping[req.varName] = new AdsVariable<uint64_t>(*m_route,req.varName);
      }
 
      break;
    }

    if(varType == "LINT")
    {
      try
      {
        res.value = *LONGRouteMapping[req.varName];
        ROS_INFO("The %s %s equals %ld",varType.c_str(),req.varName.c_str(),int64_t(res.value));
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete LONGRouteMapping[req.varName];
        LONGRouteMapping[req.varName] = new AdsVariable<int64_t>(*m_route,req.varName);
      }
 
      break;
    }

    if(varType == "REAL")
    {
      try
      {
        res.value = *FLOATRouteMapping[req.varName];
        ROS_INFO("The %s %s equals %f",varType.c_str(),req.varName.c_str(),(float)res.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete FLOATRouteMapping[req.varName];
        FLOATRouteMapping[req.varName] = new AdsVariable<float>(*m_route,req.varName);
      }
 
      break;
    }

    if(varType == "LREAL")
    {
      try
      {
        res.value = *DOUBLERouteMapping[req.varName];
        ROS_INFO("The %s %s equals %lf",varType.c_str(),req.varName.c_str(),res.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete DOUBLERouteMapping[req.varName];
        DOUBLERouteMapping[req.varName] = new AdsVariable<double>(*m_route,req.varName);
      }
 
      break;
    }

    if(varType == "DATE")
    {
      try
      {
        res.value = *UDINTRouteMapping[req.varName];
        ros::Time currentDate(*UDINTRouteMapping[req.varName]);
        time_t tDate(currentDate.toSec());
        tm tmDate;
        gmtime_r(&tDate,&tmDate);
        ROS_INFO("%s equals : %d/%d/%d %d:%d:%d",
             req.varName.c_str(),
             tmDate.tm_mday,
             tmDate.tm_mon + 1,
             tmDate.tm_year + 1900,
             tmDate.tm_hour,
             tmDate.tm_min,
             tmDate.tm_sec);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete UDINTRouteMapping[req.varName];
        UDINTRouteMapping[req.varName] = new AdsVariable<uint32_t>(*m_route,req.varName);
      }
 
      break;
    }

    if(varType == "TIME" || varType == "TIME_OF_DAY" || varType == "LTIME")
    {
      try
      {
        res.value = *UDINTRouteMapping[req.varName];
        long lTime = (long)(*UDINTRouteMapping[req.varName]);
        long lMs = lTime % 1000;
        long lSec = (lTime/1000) % 60;
        long lMin = (lTime/60000) % 60;
        long lHeu = (lTime/3600000) % 24;
        ROS_INFO("%s equals %luh%lum%lus%lums",req.varName.c_str(),lHeu,lMin,lSec,lMs);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete UDINTRouteMapping[req.varName];
        UDINTRouteMapping[req.varName] = new AdsVariable<uint32_t>(*m_route,req.varName);
      }
 
      break;
    }

    ROS_ERROR("Variable type not correct");
    bresult = false;
    break;
  }
  while(false);

  else
  {
    try
    {
      RouteMapping[req.varName]->ReadValue(res.value); 
    }
    catch(AdsException e)
    {
      ROS_ERROR_STREAM(e.what());
      bresult = false;
    }
  }

  m_ComMutex.unlock();

  return bresult;
}

bool RosAds_server_node::bindPLcVar()
{
  bool bresult = false;

  if (doc.LoadFile(m_PLCFileDefinitionPath.c_str()))
  {
    ROS_INFO("Reading your PLC config file ...");
    TiXmlNode* firstNode = doc.FirstChildElement( "Root" )->FirstChildElement( "Variable" );

    //Read each alias with corresponding ADS name
    for(TiXmlNode* currentNode = firstNode ; currentNode ; currentNode = currentNode->NextSiblingElement())
    {

      string alias = currentNode->FirstChildElement( "alias" )->GetText();
      string adsName =currentNode->FirstChildElement( "ads_name" )->GetText(); 

      //Check if ADS name is part of downloaded PLC ADS list
      if ( m_VariableADS.find(adsName) == m_VariableADS.end() )
      {
        ROS_ERROR("Error on ADS alias: %s -> %s",alias.c_str(),adsName.c_str());
        continue;
      }


      ROS_INFO("ADS alias found: %s -> %s",alias.c_str(),adsName.c_str());

      //ROS_INFO("ADS alias found: %s -> %s",alias.c_str(),adsName.c_str());


      string type = m_VariableADS[adsName];
      VariableMapping[alias] = type;
      do{
        if(type == "BOOL"){

          BOOLRouteMapping[alias] = new AdsVariable<bool>(*m_route, adsName);
          break;
        }
        if(type == "BYTE" || type == "USINT"){
          BYTERouteMapping[alias] = new AdsVariable<uint8_t>(*m_route, adsName);
          break;
        }
        if(type == "SINT"){
          SINTRouteMapping[alias] = new AdsVariable<int8_t>(*m_route, adsName);
          break;
        }
        if(type == "WORD" || type == "UINT"){
          UINTRouteMapping[alias] = new AdsVariable<uint16_t>(*m_route, adsName);
          break;
        }
        if(type == "INT"){
          INTRouteMapping[alias] = new AdsVariable<int16_t>(*m_route, adsName);
          break;
        }
        if(type == "DWORD" || type == "UDINT" || type == "DATE" || type == "TIME" || type == "TIME_OF_DAY" || type == "LTIME"){
          UDINTRouteMapping[alias] = new AdsVariable<uint32_t>(*m_route, adsName);
          break;
        }
        if(type == "DINT"){
          DINTRouteMapping[alias] = new AdsVariable<int32_t>(*m_route, adsName);
          break;
        }
        if(type == "LWORD" || type == "ULINT"){
          ULONGRouteMapping[alias] = new AdsVariable<uint64_t>(*m_route, adsName);
          break;
        }
        if(type == "LINT"){
          LONGRouteMapping[alias] = new AdsVariable<int64_t>(*m_route, adsName);
          break;
        }
        if(type == "REAL"){
          FLOATRouteMapping[alias] = new AdsVariable<float>(*m_route, adsName);
          break;
        }
        if(type == "LREAL"){
          DOUBLERouteMapping[alias] = new AdsVariable<double>(*m_route, adsName);

          RouteMapping[alias] = new AdsVariable<bool>(*m_route, adsName);
          break;
        }
        if(type == "BYTE" || type == "USINT"){
          RouteMapping[alias] = new AdsVariable<uint8_t>(*m_route, adsName);
          break;
        }
        if(type == "SINT"){
          RouteMapping[alias] = new AdsVariable<int8_t>(*m_route, adsName);
          break;
        }
        if(type == "WORD" || type == "UINT"){
          RouteMapping[alias] = new AdsVariable<uint16_t>(*m_route, adsName);
          break;
        }
        if(type == "INT"){
          RouteMapping[alias] = new AdsVariable<int16_t>(*m_route, adsName);
          break;
        }
        if(type == "DWORD" || type == "UDINT" || type == "DATE" || type == "TIME" || type == "TIME_OF_DAY" || type == "LTIME"){
          RouteMapping[alias] = new AdsVariable<uint32_t>(*m_route, adsName);
          break;
        }
        if(type == "DINT"){
          RouteMapping[alias] = new AdsVariable<int32_t>(*m_route, adsName);
          break;
        }
        if(type == "LWORD" || type == "ULINT"){
          RouteMapping[alias] = new AdsVariable<uint64_t>(*m_route, adsName);
          break;
        }
        if(type == "LINT"){
          RouteMapping[alias] = new AdsVariable<int64_t>(*m_route, adsName);
          break;
        }
        if(type == "REAL"){
          RouteMapping[alias] = new AdsVariable<float>(*m_route, adsName);
          break;
        }
        if(type == "LREAL"){
          RouteMapping[alias] = new AdsVariable<double>(*m_route, adsName);

          break;
        }
      }
      while(false);
    }
    bresult = true;
  }
  else
  {
    ROS_INFO("Failed to load the PLC config file");
  }

    return bresult;
}

bool RosAds_server_node::adsReadVariables(beckhoff_plc_control::ADSReadVariables::Request  &req, beckhoff_plc_control::ADSReadVariables::Response &res)
{

  res.size = VariableMapping.size();
  res.varNames = vector<string>();
  res.varTypes = vector<string>();

  for (auto& x: VariableMapping) {
    res.varNames.push_back(x.first);
    res.varTypes.push_back(x.second);
  }

  return true;
}

//------------------ Creating route to remote PLC --------------------

int RosAds_server_node::initRoute()
{
  m_AmsNetIdremoteNetId= new AmsNetId(m_remoteNetId);
  char remoteIpV4[m_remoteIpV4.length()+1];
  strcpy(remoteIpV4, m_remoteIpV4.c_str());
  ROS_ERROR_STREAM(m_AmsNetIdremoteNetId);
  m_route = new AdsDevice(m_remoteIpV4,*m_AmsNetIdremoteNetId, AMSPORT_R0_PLC_TC3);
  return 0;
}


//--------------- Initialisation ROS ------------------------------
int main(int argc, char **argv)
{
  RosAds_server_node *node = new RosAds_server_node();
  return node->main(argc,argv);
}



int RosAds_server_node::main(int argc, char **argv)
{

  ros::init(argc, argv, "RosAds_server");
  ros::NodeHandle n;
  ros::NodeHandle nprive("~");

  if(nprive.hasParam("localNetId"))
  {

    nprive.getParam("localNetId", m_localNetId_param);
    AdsSetLocalAddress(m_localNetId_param);
  }
  else {
    ROS_ERROR("Param localNetId unknown");
    return 0;
  }

  //------------------ Routing to remote PLC --------------------
  if(nprive.hasParam("remoteNetId"))
  {
    nprive.getParam("remoteNetId", m_remoteNetId);
  }
  else {
    ROS_ERROR("Param remoteNetId unknown");
    return 0;
  }


  if(nprive.hasParam("remoteIpV4"))
  {
    nprive.getParam("remoteIpV4", m_remoteIpV4);
  }
  else {
    ROS_ERROR("Param remoteIpV4 unknown");
    return 0;
  }


  if(nprive.hasParam("PLCPathFile"))
  {
    nprive.getParam("PLCPathFile", m_PLCFileDefinitionPath);
  }
  else {
    ROS_ERROR("Param PLCPathFile unknown");
    return 0;
  }


  ROS_ERROR_STREAM("GO FOR Init Route");
  initRoute();
  ROS_ERROR("Init Route done");

  ROS_INFO_STREAM("GO FOR Init Route");
  initRoute();
  ROS_INFO("Init Route done");


  try
  {
    m_VariableADS = m_route->GetDeviceAdsVariables();

    /*for(std::map<string,string>::iterator it = m_VariableADS.begin(); it != m_VariableADS.end(); ++it)
    {
      ROS_INFO("%s\t%s",it->first.c_str(),it->second.c_str());
    }*/
    bindPLcVar();

    m_writingValueService = n.advertiseService("ADS_write_value", &RosAds_server_node::adsWriteValue,this);
    m_readingValueService = n.advertiseService("ADS_read_value", &RosAds_server_node::adsReadValue,this);
    m_readingVariablesService = n.advertiseService("ADS_read_variables", &RosAds_server_node::adsReadVariables,this);

    ROS_INFO("Ready to communicate with the remote PLC via ADS.");
  }
  catch(AdsException e)
  {
    ROS_INFO_STREAM(e.what());
    ROS_ERROR("ERROR in mapping alias with ADS");
  }
  

  beckhoff_plc_control::ADSReadValue::Request  req;
  beckhoff_plc_control::ADSReadValue::Response res;

  req.varName = "data1"; adsReadValue(req,res);
  ROS_INFO("%s\t%f",req.varName.c_str(),(float)res.value);

req.varName = "data2"; adsReadValue(req,res);
  ROS_INFO("%s\t%f",req.varName.c_str(),(float)res.value);

req.varName = "data3"; adsReadValue(req,res);
  ROS_INFO("%s\t%f",req.varName.c_str(),(float)res.value);

req.varName = "data4"; adsReadValue(req,res);
  ROS_INFO("%s\t%f",req.varName.c_str(),(float)res.value);

req.varName = "data5"; adsReadValue(req,res);
  ROS_INFO("%s\t%f",req.varName.c_str(),(float)res.value);

req.varName = "data6"; adsReadValue(req,res);
  ROS_INFO("%s\t%f",req.varName.c_str(),(float)res.value);

req.varName = "data7"; adsReadValue(req,res);
  ROS_INFO("%s\t%f",req.varName.c_str(),(float)res.value);

req.varName = "data8"; adsReadValue(req,res);
  ROS_INFO("%s\t%f",req.varName.c_str(),(float)res.value);

req.varName = "data9"; adsReadValue(req,res);
  ROS_INFO("%s\t%f",req.varName.c_str(),(float)res.value);

  
  try
  {
    m_VariableADS = m_route->GetDeviceAdsVariables();

    for(std::map<string,string>::iterator it = m_VariableADS.begin(); it != m_VariableADS.end(); ++it)
    {
      ROS_INFO("%s\t%s",it->first.c_str(),it->second.c_str());
    }
    bindPLcVar();

    m_writingValueService = n.advertiseService("ADS_write_value", &RosAds_server_node::adsWriteValue,this);
    m_readingValueService = n.advertiseService("ADS_read_value", &RosAds_server_node::adsReadValue,this);
    m_readingVariablesService = n.advertiseService("ADS_read_variables", &RosAds_server_node::adsReadVariables,this);
  
    ROS_INFO("Ready to communicate with the remote PLC via ADS.");
  }
  catch(AdsException e)
  {
	ROS_INFO_STREAM(e.what());
	
	ROS_INFO("ERROR in mapping alias with ADS.");
  }
  

  ros::spin();

  return 0;
}
