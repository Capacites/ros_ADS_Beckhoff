#include "Ads_Interface.h"

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

bool RosAds_server_node::adsWriteValue(ros_ads_msgs::ADSWriteValue  &req, ros_ads_msgs::ADSWriteValue &res){


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
            *RouteMapping[req.varName] = false;
            ROS_INFO("The %s %s now equals false",varType.c_str(),req.varName.c_str());
          }else{
            *RouteMapping[req.varName] = true;
            ROS_INFO("The %s %s now equals true",varType.c_str(),req.varName.c_str());
          }
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete RouteMapping[req.varName];
          RouteMapping[req.varName] = new AdsVariable<bool>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
    }
    catch(AdsException e)
    {

    }


    if(varType == "BYTE" || varType == "USINT")
    {
      if((uint8_t)req.value == req.value)
      {
        try
        {
          *RouteMapping[req.varName] = (uint8_t)req.value;
          ROS_INFO("The %s %s now equals %u",varType.c_str(),req.varName.c_str(),(uint8_t)req.value);
        }
	catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          factory(req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
    }

    if(varType == "SINT")
    {
      if((int8_t)req.value == req.value)
      {
        try
        {
          *RouteMapping[req.varName] = (int8_t)req.value;
          ROS_INFO("The %s %s now equals %d",varType.c_str(),req.varName.c_str(),(int8_t)req.value);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete RouteMapping[req.varName];
          RouteMapping[req.varName] = new AdsVariable<int8_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
    }

    if(varType == "WORD" || varType == "UINT")
    {
      if((uint16_t)req.value == req.value)
      {
        try
        {
          *RouteMapping[req.varName] = (uint16_t)req.value;
          ROS_INFO("The %s %s now equals %u",varType.c_str(),req.varName.c_str(),(uint16_t)req.value);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete RouteMapping[req.varName];
          RouteMapping[req.varName] = new AdsVariable<uint16_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
    }

    if(varType == "INT")
    {
      if((int16_t)req.value == req.value)
      {
        try
        {
          *RouteMapping[req.varName] = (int16_t)req.value;
         ROS_INFO("The %s %s now equals %d",varType.c_str(),req.varName.c_str(),(int16_t)req.value);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete RouteMapping[req.varName];
          RouteMapping[req.varName] = new AdsVariable<int16_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
    }

    if(varType == "DWORD" || varType == "UDINT")
    {
      if((uint32_t)req.value == req.value)
      {
        try
        {
          *RouteMapping[req.varName] = (uint32_t)req.value;
          ROS_INFO("The %s %s now equals %u",varType.c_str(),req.varName.c_str(),(uint32_t)req.value);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete RouteMapping[req.varName];
          RouteMapping[req.varName] = new AdsVariable<uint32_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
    }

    if(varType == "DINT")
    {
      if((int32_t)req.value == req.value)
      {
        try
        {
          *RouteMapping[req.varName] = (int32_t)req.value;
          ROS_INFO("The %s %s now equals %d",varType.c_str(),req.varName.c_str(),(int32_t)req.value);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete RouteMapping[req.varName];
          RouteMapping[req.varName] = new AdsVariable<int32_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
    }

    if(varType == "LWORD" || varType == "ULINT")
    {
      if((uint64_t)req.value == req.value)
      {
        try
        {
          *RouteMapping[req.varName] = (uint64_t)req.value;
          ROS_INFO("The %s %s now equals %lu",varType.c_str(),req.varName.c_str(),(uint64_t)req.value);
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete RouteMapping[req.varName];
          RouteMapping[req.varName] = new AdsVariable<uint64_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
    }

    if(varType == "LINT")
    {
      if((int64_t)req.value == req.value)
      {
        try
        {
          *RouteMapping[req.varName] = (int64_t)req.value;
          ROS_INFO("The %s %s now equals %ld",varType.c_str(),req.varName.c_str(),int64_t(req.value));
        }
        catch(AdsException e)
        {
          ROS_ERROR_STREAM(e.what());
          delete RouteMapping[req.varName];
          RouteMapping[req.varName] = new AdsVariable<int64_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }
    }

    if(varType == "REAL")
    {
      try
      {
        *RouteMapping[req.varName] = (float)req.value;
        ROS_INFO("The %s %s now equals %f",varType.c_str(),req.varName.c_str(),(float)req.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete RouteMapping[req.varName];
        RouteMapping[req.varName] = new AdsVariable<float>(*m_route,req.varName);
      }
    }

    if(varType == "LREAL")
    {
      try
      {
        *RouteMapping[req.varName] = req.value;
        ROS_INFO("The %s %s now equals %lf",varType.c_str(),req.varName.c_str(),req.value);
      }
      catch(AdsException e)
      {
        ROS_ERROR_STREAM(e.what());
        delete RouteMapping[req.varName];
        RouteMapping[req.varName] = new AdsVariable<double>(*m_route,req.varName);
      }
    }

    if(varType == "DATE")
    {
      if((uint32_t)req.value == req.value)
      {
        try
        {
          *RouteMapping[req.varName] = (uint32_t)req.value;
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
          delete RouteMapping[req.varName];
          RouteMapping[req.varName] = new AdsVariable<uint32_t>(*m_route,req.varName);
        }
      }
      else
      {
        dataCorrect = false;
      }

      *RouteMapping[req.varName] = req.value;
      //ROS_INFO("The %s %s now equals true",varType.c_str(),req.varName.c_str());

    }

  }
  m_ComMutex.unlock();
  if(!dataCorrect) {
    ROS_ERROR("Data size not correct");
    bresult =  false;
  }
  return bresult;
}

bool RosAds_server_node::adsReadValue(ros_ads_msgs::ADSReadValue  &req, ros_ads_msgs::ADSReadValue &res)
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
      try
      {
        RouteMapping[req.varName]->ReadValue(res.value);
        ROS_INFO("The %s %s equals %f",varType.c_str(),req.varName.c_str(), res.value);
      }
      catch(AdsException e)
      {
        bresult = false;
        ROS_ERROR_STREAM(e.what());
        factory(req.varName);
      }
      break;
    }
  while(0);
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

      string type = m_VariableADS[adsName];
      VariableMapping[alias] = type;
      do{
        if(type == "BOOL"){

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

bool RosAds_server_node::adsReadVariables(ros_ads_msgs::ADSReadVariables  &req, ros_ads_msgs::ADSReadVariables &res)
{
  res.size = VariableMapping.size();
  res.varNames = vector<string>();
  res.varTypes = vector<string>();

  for (auto& x: VariableMapping)
  {
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

  try
  {
    m_VariableADS = m_route->GetDeviceAdsVariables();

    /*for(std::map<string,string>::iterator it = m_VariableADS.begin(); it != m_VariableADS.end(); ++it)
    {
      ROS_INFO("%s\t%s",it->first.c_str(),it->second.c_str());
    }*/
    bindPLcVar();

    //m_writingValueService = n.advertiseService("ADS_write_value", &RosAds_server_node::adsWriteValue,this);
    //m_readingValueService = n.advertiseService("ADS_read_value", &RosAds_server_node::adsReadValue,this);
    //m_readingVariablesService = n.advertiseService("ADS_read_variables", &RosAds_server_node::adsReadVariables,this);

    ROS_INFO("Ready to communicate with the remote PLC via ADS.");
  }
  catch(AdsException e)
  {
    ROS_INFO_STREAM(e.what());
    ROS_ERROR("ERROR in mapping alias with ADS");
  }
  

  ros_ads_msgs::ADSReadValue  req;
  ros_ads_msgs::ADSReadValue res;

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

    //m_writingValueService = n.advertiseService("ADS_write_value", &RosAds_server_node::adsWriteValue,this);
    //m_readingValueService = n.advertiseService("ADS_read_value", &RosAds_server_node::adsReadValue,this);
    //m_readingVariablesService = n.advertiseService("ADS_read_variables", &RosAds_server_node::adsReadVariables,this);
  
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

bool RosAds_server_node::factory(string  varName)
{
    bool result = false;
    try {
        string type = m_VariableADS[varName];
        do{
          if (RouteMapping.find(varName) != RouteMapping.end())
          {
              delete RouteMapping[varName];
          }
          if(type == "BOOL"){
            RouteMapping[varName] = new AdsVariable<bool>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "BYTE" || type == "USINT"){
            RouteMapping[varName] = new AdsVariable<uint8_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "SINT"){
            RouteMapping[varName] = new AdsVariable<int8_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "WORD" || type == "UINT"){
            RouteMapping[varName] = new AdsVariable<uint16_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "INT"){
            RouteMapping[varName] = new AdsVariable<int16_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "DWORD" || type == "UDINT" || type == "DATE" || type == "TIME" || type == "TIME_OF_DAY" || type == "LTIME"){
            RouteMapping[varName] = new AdsVariable<uint32_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "DINT"){
            RouteMapping[varName] = new AdsVariable<int32_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "LWORD" || type == "ULINT"){
           RouteMapping[varName] = new AdsVariable<uint64_t>(*m_route,varName);
           result = true;
            break;
          }
          if(type == "LINT"){
            RouteMapping[varName] = new AdsVariable<int64_t>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "REAL"){
            RouteMapping[varName] = new AdsVariable<float>(*m_route,varName);
            result = true;
            break;
          }
          if(type == "LREAL"){
            RouteMapping[varName] = new AdsVariable<double>(*m_route,varName);
            result = true;
            break;
          }
        }
        while(false);
    } catch (AdsException e) {
        ROS_INFO("error factory");
    }

    return result;
}
