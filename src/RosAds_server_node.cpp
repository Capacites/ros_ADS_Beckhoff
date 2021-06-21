
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
do
  {

    //La variable n'existe pas
    if(varType == ""){
      ROS_ERROR("Variable name not correct");
      dataCorrect =  false;
      bresult =  false;
      break;
    }


    if(varType == "BOOL")
    {
      if((uint8_t)req.value == req.value)
      {
	if(BOOLRouteMapping.find(req.varName) == BOOLRouteMapping.end()){
		AdsVariable<bool> *var = new AdsVariable<bool>(*m_route, req.varName);
		BOOLRouteMapping[req.varName] = var;
	}
        if((uint8_t)req.value == 0){
          *BOOLRouteMapping[req.varName] = false;
          ROS_INFO("The %s %s now equals false",varType.c_str(),req.varName.c_str());
        }else{
          *BOOLRouteMapping[req.varName] = true;
          ROS_INFO("The %s %s now equals true",varType.c_str(),req.varName.c_str());
        }
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }


    if(varType == "BYTE")
    {
      if((uint8_t)req.value == req.value)
      {
        if(BYTERouteMapping.find(req.varName) == BYTERouteMapping.end()){
		AdsVariable<uint8_t> *var = new AdsVariable<uint8_t>(*m_route, req.varName);
		BYTERouteMapping[req.varName] = var;
	}
        *BYTERouteMapping[req.varName] = (uint8_t)req.value;
        ROS_INFO("The %s %s now equals %u",varType.c_str(),req.varName.c_str(),(uint8_t)req.value);
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
        if(USINTRouteMapping.find(req.varName) == USINTRouteMapping.end()){
		AdsVariable<uint16_t> *var = new AdsVariable<uint16_t>(*m_route, req.varName);
		USINTRouteMapping[req.varName] = var;
	}
        *USINTRouteMapping[req.varName] = (uint16_t)req.value;
        ROS_INFO("The %s %s now equals %u",varType.c_str(),req.varName.c_str(),(uint16_t)req.value);
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
        if(SINTRouteMapping.find(req.varName) == SINTRouteMapping.end()){
		AdsVariable<int16_t> *var  = new AdsVariable<int16_t>(*m_route, req.varName);
		SINTRouteMapping[req.varName] = var;
	}
        *SINTRouteMapping[req.varName] = (int16_t)req.value;
        ROS_INFO("The %s %s now equals %d",varType.c_str(),req.varName.c_str(),(int16_t)req.value);
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
        if(UINTRouteMapping.find(req.varName) == UINTRouteMapping.end()){
		AdsVariable<uint32_t> *var = new AdsVariable<uint32_t>(*m_route, req.varName);
		UINTRouteMapping[req.varName] = var;
	}
        *UINTRouteMapping[req.varName] = (uint32_t)req.value;
        ROS_INFO("The %s %s now equals %u",varType.c_str(),req.varName.c_str(),(uint32_t)req.value);
      }else
      {
        dataCorrect = false;
      }
      break;
    }


    if(varType == "DINT")
    {
      if((int32_t)req.value == req.value)
      {
        if(INTRouteMapping.find(req.varName) == INTRouteMapping.end()){
		AdsVariable<int32_t> *var = new AdsVariable<int32_t>(*m_route, req.varName);
		INTRouteMapping[req.varName] = var;
	}
        *INTRouteMapping[req.varName] = (int32_t)req.value;
        ROS_INFO("The %s %s now equals %d",varType.c_str(),req.varName.c_str(),(int32_t)req.value);
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
        if(ULONGRouteMapping.find(req.varName) == ULONGRouteMapping.end()){
		AdsVariable<uint64_t> *var = new AdsVariable<uint64_t>(*m_route, req.varName);
		ULONGRouteMapping[req.varName] = var;
	}
        *ULONGRouteMapping[req.varName] = (uint64_t)req.value;
        ROS_INFO("The %s %s now equals %lu",varType.c_str(),req.varName.c_str(),(uint64_t)req.value);
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }

    if(varType == "LINT")
    {
      if((int64_t)req.value == req.value){
        if(LONGRouteMapping.find(req.varName) == LONGRouteMapping.end()){
		AdsVariable<int64_t> *var = new AdsVariable<int64_t>(*m_route, req.varName);
		LONGRouteMapping[req.varName] = var;
	}
        *LONGRouteMapping[req.varName] = (int64_t)req.value;
        ROS_INFO("The %s %s now equals %ld",varType.c_str(),req.varName.c_str(),int64_t(req.value));
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }

    if(varType == "REAL")
    {
      if((float)req.value/2 != (float)req.value)
      {
        if(FLOATRouteMapping.find(req.varName) == FLOATRouteMapping.end()){
		AdsVariable<float> *var =new AdsVariable<float>(*m_route, req.varName);
		FLOATRouteMapping[req.varName] = var;
	}
        *FLOATRouteMapping[req.varName] = (float)req.value;
        ROS_INFO("The %s %s now equals %f",varType.c_str(),req.varName.c_str(),(float)req.value);
      }
      else
      {
        dataCorrect = false;
      }
      break;
    }

    if(varType == "LREAL")
    {
      if(DOUBLERouteMapping.find(req.varName) == DOUBLERouteMapping.end()){
		AdsVariable<double> *var = new AdsVariable<double>(*m_route, req.varName);
		DOUBLERouteMapping[req.varName] = var;
	}
      *DOUBLERouteMapping[req.varName] = req.value;
      ROS_INFO("The %s %s now equals %lf",varType.c_str(),req.varName.c_str(),req.value);
      break;
    }

    if(varType == "DATE")
    {
      if((uint32_t)req.value == req.value)
      {
        if(UINTRouteMapping.find(req.varName) == UINTRouteMapping.end()){
		AdsVariable<uint32_t> *var = new AdsVariable<uint32_t>(*m_route, req.varName);
		UINTRouteMapping[req.varName] = var;
	}
        *UINTRouteMapping[req.varName] = (uint32_t)req.value;
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
      else
      {
        dataCorrect = false;
      }
      break;
    }

    if(varType == "TIME" || varType == "TIME_OF_DAY" || varType == "LTIME")
    {
      if((uint32_t)req.value == req.value)
      {

        if(UINTRouteMapping.find(req.varName) == UINTRouteMapping.end()){
		AdsVariable<uint32_t> *var = new AdsVariable<uint32_t>(*m_route, req.varName);
		UINTRouteMapping[req.varName] = var;
	}
        if(varType == "TIME" || varType == "TIME_OF_DAY")
        {
        	*UINTRouteMapping[req.varName] = (uint32_t)req.value;
        }else if(varType == "LTIME")
        {
        	*UINTRouteMapping[req.varName] = (uint32_t)(req.value * 1000000);
        }
        long lTime = (long)(req.value);
        long lMs = lTime % 1000;
        long lSec = (lTime/1000) % 60;
        long lMin = (lTime/60000) % 60;
        long lHeu = (lTime/3600000) % 24;
        ROS_INFO("%s set to %luh%lum%lus%lums",req.varName.c_str(),lHeu,lMin,lSec,lMs);
      }else
      {
        dataCorrect = false;
      }
      break;
    }
  }while(false);
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

  if(varType == ""){
    ROS_ERROR("Variable name not correct");
    bresult = false;
    }
do{
    m_ComMutex.lock();
  if(varType == "BOOL")
  {
    if(BOOLRouteMapping.find(req.varName) == BOOLRouteMapping.end()){
		AdsVariable<bool> *var = new AdsVariable<bool>(*m_route, req.varName);
		BOOLRouteMapping[req.varName] = var;
	}
    res.value = *BOOLRouteMapping[req.varName];
    if(*BOOLRouteMapping[req.varName] == false)
    {
      ROS_INFO("The %s %s equals false",varType.c_str(),req.varName.c_str());
    }
    else
    {
      ROS_INFO("The %s %s equals true",varType.c_str(),req.varName.c_str());
    }
    break;
  }
  if(varType == "BYTE")
  {
    if(BYTERouteMapping.find(req.varName) == BYTERouteMapping.end()){
		AdsVariable<uint8_t> *var = new AdsVariable<uint8_t>(*m_route, req.varName);
		BYTERouteMapping[req.varName] = var;
	}
    res.value = *BYTERouteMapping[req.varName];
    ROS_INFO("The %s %s equals %u",varType.c_str(),req.varName.c_str(),(uint8_t)res.value);
    break;
  }

  if(varType == "WORD" || varType == "UINT")
  {
    if(USINTRouteMapping.find(req.varName) == USINTRouteMapping.end()){
		AdsVariable<uint16_t> *var = new AdsVariable<uint16_t>(*m_route, req.varName);
		USINTRouteMapping[req.varName] = var;
	}
    res.value = *USINTRouteMapping[req.varName];
    ROS_INFO("The %s %s equals %u",varType.c_str(),req.varName.c_str(),(uint16_t)res.value);
    break;
  }

  if(varType == "INT")
  {
    if(SINTRouteMapping.find(req.varName) == SINTRouteMapping.end()){
		AdsVariable<int16_t> *var =new AdsVariable<int16_t>(*m_route, req.varName);
		SINTRouteMapping[req.varName] = var;
 	}
    res.value = *SINTRouteMapping[req.varName];
    ROS_INFO("The %s %s equals %d",varType.c_str(),req.varName.c_str(),(int16_t)res.value);
    break;
  }

  if(varType == "DWORD" || varType == "UDINT")
  {
    if(UINTRouteMapping.find(req.varName) == UINTRouteMapping.end()){
		AdsVariable<uint32_t> *var = new AdsVariable<uint32_t>(*m_route, req.varName);
		UINTRouteMapping[req.varName] = var;
	}
    res.value = *UINTRouteMapping[req.varName];
    ROS_INFO("The %s %s equals %u",varType.c_str(),req.varName.c_str(),(uint32_t)res.value);
    break;
  }

  if(varType == "DINT")
  {
    if(INTRouteMapping.find(req.varName) == INTRouteMapping.end()){
		AdsVariable<int32_t> *var = new AdsVariable<int32_t>(*m_route, req.varName);
		INTRouteMapping[req.varName] = var;
	}
    res.value = *INTRouteMapping[req.varName];
    ROS_INFO("The %s %s equals %d",varType.c_str(),req.varName.c_str(),(int32_t)res.value);
    break;
  }

  if(varType == "LWORD" || varType == "ULINT")
  {
    if(ULONGRouteMapping.find(req.varName) == ULONGRouteMapping.end()){
		AdsVariable<uint64_t> *var = new AdsVariable<uint64_t>(*m_route, req.varName);
		ULONGRouteMapping[req.varName] = var;
	}
    res.value = *ULONGRouteMapping[req.varName];
    ROS_INFO("The %s %s equals %lu",varType.c_str(),req.varName.c_str(),(uint64_t)res.value);
    break;
  }

  if(varType == "LINT")
  {
    if(LONGRouteMapping.find(req.varName) == LONGRouteMapping.end()){
		AdsVariable<int64_t> *var = new AdsVariable<int64_t>(*m_route, req.varName);
		LONGRouteMapping[req.varName] = var;
	}
    res.value = *LONGRouteMapping[req.varName];
    ROS_INFO("The %s %s equals %ld",varType.c_str(),req.varName.c_str(),int64_t(res.value));
    break;
  }

  if(varType == "REAL")
  {
    if(FLOATRouteMapping.find(req.varName) == FLOATRouteMapping.end()){
		AdsVariable<float> *var = new AdsVariable<float>(*m_route, req.varName);
		FLOATRouteMapping[req.varName] = var;
	}
    res.value = *FLOATRouteMapping[req.varName];
    ROS_INFO("The %s %s equals %f",varType.c_str(),req.varName.c_str(),(float)res.value);
    break;
  }

  if(varType == "LREAL")
  {
    if(DOUBLERouteMapping.find(req.varName) == DOUBLERouteMapping.end()){
		AdsVariable<double> *var = new AdsVariable<double>(*m_route, req.varName);
		DOUBLERouteMapping[req.varName] = var;
	}
    res.value = *DOUBLERouteMapping[req.varName];
    ROS_INFO("The %s %s equals %lf",varType.c_str(),req.varName.c_str(),res.value);
    break;
  }

  if(varType == "DATE")
  {
    if(UINTRouteMapping.find(req.varName) == UINTRouteMapping.end()){
		AdsVariable<uint32_t> *var = new AdsVariable<uint32_t>(*m_route, req.varName);
		UINTRouteMapping[req.varName] = var;
	}
    res.value = *UINTRouteMapping[req.varName];
    ros::Time currentDate(*UINTRouteMapping[req.varName]);
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
    break;
  }

  if(varType == "TIME" || varType == "TIME_OF_DAY" || varType == "LTIME")
  {
    if(UINTRouteMapping.find(req.varName) == UINTRouteMapping.end()){
		AdsVariable<uint32_t> *var = new AdsVariable<uint32_t>(*m_route, req.varName);
		UINTRouteMapping[req.varName] = var;
	}
    res.value = *UINTRouteMapping[req.varName];
    long lTime = (long)(*UINTRouteMapping[req.varName]);
    long lMs = lTime % 1000;
    long lSec = (lTime/1000) % 60;
    long lMin = (lTime/60000) % 60;
    long lHeu = (lTime/3600000) % 24;
    ROS_INFO("%s equals %luh%lum%lus%lums",req.varName.c_str(),lHeu,lMin,lSec,lMs);
    break;
  }

  m_ComMutex.unlock();
  ROS_ERROR("Variable type not correct");
  bresult = false;
  break;
  }
  while(false);

  return bresult;
}


bool RosAds_server_node::loadPLcVar()
{
  bool bresult = false;

  if (doc.LoadFile(m_PLCFileDefinitionPath.c_str()))
  {
    ROS_INFO("Reading your PLC config file ...");
    TiXmlNode* firstNode = doc.FirstChildElement( "PlcProjectInfo" )->FirstChildElement( "Symbols" )->FirstChildElement( "Symbol" );

    for(TiXmlNode* currentNode = firstNode ; currentNode ; currentNode = currentNode->NextSiblingElement())
    {
      string name = currentNode->FirstChildElement( "Name" )->GetText();
      string type =currentNode->FirstChildElement( "Type" )->GetText();
      VariableMapping[name] = type;
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

int main(int argc, char **argv)
{
  RosAds_server_node *node = new RosAds_server_node();
  return node->main(argc,argv);
}

int RosAds_server_node::initRoute()
{
  //------------------ Routing to remote PLC --------------------


  m_AmsNetIdremoteNetId= new AmsNetId(m_remoteNetId);
  char remoteIpV4[m_remoteIpV4.length()+1];
  strcpy(remoteIpV4, m_remoteIpV4.c_str());
  m_route = new AdsDevice(m_remoteIpV4,*m_AmsNetIdremoteNetId, AMSPORT_R0_PLC_TC3);
  return 0;
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
    AdsSetLocalAddress(m_remoteNetId);
  }
  else {
    ROS_ERROR("Param remoteNetId unknown");
    return 0;
  }


  if(nprive.hasParam("remoteIpV4"))
  {

    nprive.getParam("remoteIpV4", m_remoteIpV4);
    AdsSetLocalAddress(m_remoteIpV4);
  }
  else {
    ROS_ERROR("Param remoteIpV4 unknown");
    return 0;
  }


  if(nprive.hasParam("PLCPathFile"))
  {

    nprive.getParam("PLCPathFile", m_remoteIpV4);
    AdsSetLocalAddress(m_remoteIpV4);
  }
  else {
    ROS_ERROR("Param PLCPathFile unknown");
    return 0;
  }

  loadPLcVar();
  initRoute();
  m_writingValueService = n.advertiseService("ADS_write_value", &RosAds_server_node::adsWriteValue,this);
  m_readingValueService = n.advertiseService("ADS_read_value", &RosAds_server_node::adsReadValue,this);
  m_readingVariablesService = n.advertiseService("ADS_read_variables", &RosAds_server_node::adsReadVariables,this);


  ROS_INFO("Ready to communicate with the remote PLC via ADS.");
  ros::spin();

  return 0;
}
