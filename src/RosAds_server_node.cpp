
#include "RosAds_server_node.h"
using namespace std;

TiXmlDocument doc;




string checkVariable(string varName){
  string varType;
  TiXmlNode* firstNode = doc.FirstChildElement( "PlcProjectInfo" )->FirstChildElement( "Symbols" )->FirstChildElement( "Symbol" );

  for(TiXmlNode* currentNode = firstNode ; currentNode ; currentNode = currentNode->NextSiblingElement()){
    if(currentNode->FirstChildElement( "Name" )->GetText() == varName){
      varType = currentNode->FirstChildElement( "Type" )->GetText();
      break;
    }
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
        AdsVariable<bool> var {*m_route, req.varName};
        if((uint8_t)req.value == 0){
          var = false;
          ROS_INFO("The %s %s now equals false",varType.c_str(),req.varName.c_str());
        }else{
          var = true;
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
        AdsVariable<uint8_t> var {*m_route, req.varName};
        var = (uint8_t)req.value;
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
        AdsVariable<uint16_t> var {*m_route, req.varName};
        var = (uint16_t)req.value;
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
        AdsVariable<int16_t> var {*m_route, req.varName};
        var = (int16_t)req.value;
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
        AdsVariable<uint32_t> var {*m_route, req.varName};
        var = (uint32_t)req.value;
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
        AdsVariable<int32_t> var {*m_route, req.varName};
        var = (int32_t)req.value;
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
        AdsVariable<uint64_t> var {*m_route, req.varName};
        var = (uint64_t)req.value;
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
        AdsVariable<int64_t> var {*m_route, req.varName};
        var = (int64_t)req.value;
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
        AdsVariable<float> var {*m_route, req.varName};
        var = (float)req.value;
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
      AdsVariable<double> var {*m_route, req.varName};
      var = req.value;
      ROS_INFO("The %s %s now equals %lf",varType.c_str(),req.varName.c_str(),req.value);
      break;
    }

    if(varType == "DATE")
    {
      if((uint32_t)req.value == req.value)
      {
        AdsVariable<uint32_t> var {*m_route, req.varName};
        var = (uint32_t)req.value;
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

        AdsVariable<uint32_t> var {*m_route, req.varName};
        if(varType == "TIME" || varType == "TIME_OF_DAY")
        {
          var = (uint32_t)req.value;
        }else if(varType == "LTIME")
        {
          var = (uint32_t)(req.value * 1000000);
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

/*

  string varType = checkVariable(req.varName);

  if(varType == ""){
    ROS_ERROR("Variable name not correct");
    return false;
    }

  if(varType == "BOOL"){

    AdsVariable<bool> var {route, req.varName};
    res.value = var;
    if(var == false){
      ROS_INFO("The %s %s equals false",varType.c_str(),req.varName.c_str());
    }else{
      ROS_INFO("The %s %s equals true",varType.c_str(),req.varName.c_str());
    }

  }else if(varType == "BYTE")
  {
    AdsVariable<uint8_t> var {route, req.varName};
    res.value = var;
    ROS_INFO("The %s %s equals %u",varType.c_str(),req.varName.c_str(),(uint8_t)res.value);

  }

  else if(varType == "WORD" || varType == "UINT")
  {
    AdsVariable<uint16_t> var {route, req.varName};
    res.value = var;
    ROS_INFO("The %s %s equals %u",varType.c_str(),req.varName.c_str(),(uint16_t)res.value);
  }
  else if(varType == "INT")
  {
    AdsVariable<int16_t> var {route, req.varName};
    res.value = var;
    ROS_INFO("The %s %s equals %d",varType.c_str(),req.varName.c_str(),(int16_t)res.value);

  }
  else if(varType == "DWORD" || varType == "UDINT")
  {
    AdsVariable<uint32_t> var {route, req.varName};
    res.value = var;
    ROS_INFO("The %s %s equals %u",varType.c_str(),req.varName.c_str(),(uint32_t)res.value);

  }else if(varType == "DINT")
  {
    AdsVariable<int32_t> var {route, req.varName};
    res.value = var;
    ROS_INFO("The %s %s equals %d",varType.c_str(),req.varName.c_str(),(int32_t)res.value);
  }
  else if(varType == "LWORD" || varType == "ULINT")
  {
    AdsVariable<uint64_t> var {route, req.varName};
    res.value = var;
    ROS_INFO("The %s %s equals %lu",varType.c_str(),req.varName.c_str(),(uint64_t)res.value);
  }
  else if(varType == "LINT")
  {
    AdsVariable<int64_t> var {route, req.varName};
    res.value = var;
    ROS_INFO("The %s %s equals %ld",varType.c_str(),req.varName.c_str(),int64_t(res.value));
  }else if(varType == "REAL")
  {

    AdsVariable<float> var {route, req.varName};
    res.value = var;
    ROS_INFO("The %s %s equals %f",varType.c_str(),req.varName.c_str(),(float)res.value);
  }
  else if(varType == "LREAL")
  {
    AdsVariable<double> var {route, req.varName};
    res.value = var;
    ROS_INFO("The %s %s equals %lf",varType.c_str(),req.varName.c_str(),res.value);

  }
  else if(varType == "DATE")
  {
    AdsVariable<uint32_t> var {route, req.varName};
    res.value = var;
    ros::Time currentDate(var);
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
  else if(varType == "TIME" || varType == "TIME_OF_DAY" || varType == "LTIME")
  {
    AdsVariable<uint32_t> var {route, req.varName};
    res.value = var;
    long lTime = (long)(var);
    long lMs = lTime % 1000;
    long lSec = (lTime/1000) % 60;
    long lMin = (lTime/60000) % 60;
    long lHeu = (lTime/3600000) % 24;
    ROS_INFO("%s equals %luh%lum%lus%lums",req.varName.c_str(),lHeu,lMin,lSec,lMs);
  }
  else
  {
    ROS_ERROR("Variable type not correct");
    return false;
  }
*/
  return true;
}

bool RosAds_server_node::adsReadVariables(beckhoff_plc_control::ADSReadVariables::Request  &req, beckhoff_plc_control::ADSReadVariables::Response &res)
{
  /*
  string filePath = ros::package::getPath("beckhoff_plc_control") +"/include/"+req.fileName;

  if (doc.LoadFile(filePath.c_str()))
  {
    ROS_INFO("Reading your PLC config file ...");
  int i=0;
  TiXmlNode* firstNode = doc.FirstChildElement( "PlcProjectInfo" )->FirstChildElement( "Symbols" )->FirstChildElement( "Symbol" );

  for(TiXmlNode* currentNode = firstNode ; currentNode ; currentNode = currentNode->NextSiblingElement()){
    i++;
  }
  res.varNames = vector<string>(i,"");
  res.varTypes = vector<string>(i,"");
  i=0;
  for(TiXmlNode* currentNode = firstNode ; currentNode ; currentNode = currentNode->NextSiblingElement()){
    res.varNames[i] = currentNode->FirstChildElement( "Name" )->GetText();
    res.varTypes[i] = currentNode->FirstChildElement( "Type" )->GetText();
    i++;
  }
    return true;
  }
  else
  {
  ROS_INFO("Failed to load the PLC config file");
  */
    return false;
 // }
}
int main(int argc, char **argv)
{

}

int RosAds_server_node::initRoute()
{
  //------------------ Routing to remote PLC --------------------


  m_AmsNetIdremoteNetId= new AmsNetId(m_remoteNetId);
  char remoteIpV4[m_remoteIpV4.length()+1];
  strcpy(remoteIpV4, m_remoteIpV4.c_str());
   m_route = new AdsDevice(m_remoteIpV4,*m_AmsNetIdremoteNetId, AMSPORT_R0_PLC_TC3);
}
int RosAds_server_node::main(int argc, char **argv)
{
  string localNetId_param;

  ros::init(argc, argv, "RosAds_server");
  ros::NodeHandle n;

  n.getParam("/RosAds_server/localNetId", localNetId_param);
  AdsSetLocalAddress(localNetId_param);

  //------------------ Routing to remote PLC --------------------
  n.getParam("/RosAds_server/remoteNetId", m_remoteNetId);
  n.getParam("/RosAds_server/remoteIpV4", m_remoteIpV4);


  initRoute();
  m_writingValueService = n.advertiseService("ADS_write_value", &RosAds_server_node::adsWriteValue,this);
  m_readingValueService = n.advertiseService("ADS_read_value", &RosAds_server_node::adsReadValue,this);
  m_readingVariablesService = n.advertiseService("ADS_read_variables", &RosAds_server_node::adsReadVariables,this);


  ROS_INFO("Ready to communicate with the remote PLC via ADS.");
  ros::spin();

  return 0;
}
