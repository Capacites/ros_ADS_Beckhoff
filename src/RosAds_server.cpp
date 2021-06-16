#include "ros/ros.h"
#include "ros/package.h"
#include "beckhoff_plc_control/ADSWriteValue.h"
#include "beckhoff_plc_control/ADSReadValue.h"
#include "beckhoff_plc_control/ADSReadVariables.h"
#include <tinyxml.h>
#include <cstdlib>
#include "AdsLib/AdsLib.h"
#include "AdsLib/AdsVariable.h"
#include <time.h>

using namespace std;

TiXmlDocument doc;
string remoteNetId_param,remoteIpV4_param;

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

bool adsWriteValue(beckhoff_plc_control::ADSWriteValue::Request  &req, beckhoff_plc_control::ADSWriteValue::Response &res, ros::NodeHandle* n){
	//------------------ Routing to remote PLC --------------------
	n->getParam("/RosAds_server/remoteNetId", remoteNetId_param);
	n->getParam("/RosAds_server/remoteIpV4", remoteIpV4_param);

	const AmsNetId remoteNetId {remoteNetId_param};
	char remoteIpV4[remoteIpV4_param.length()+1];
	strcpy(remoteIpV4, remoteIpV4_param.c_str());
	AdsDevice route {remoteIpV4, remoteNetId, AMSPORT_R0_PLC_TC3};
	//-------------------------------------------------------------

	string varType = checkVariable(req.varName);
	bool dataCorrect = true;

	if(varType == ""){
		ROS_ERROR("Variable name not correct");
		return false;
	}

	if(varType == "BOOL"){
		if((uint8_t)req.value == req.value){
			AdsVariable<bool> var {route, req.varName};
			if((uint8_t)req.value == 0){
				var = false;
				ROS_INFO("The %s %s now equals false",varType.c_str(),req.varName.c_str());
			}else{
				var = true;
				ROS_INFO("The %s %s now equals true",varType.c_str(),req.varName.c_str());
			}
		}else dataCorrect = false;
	  }else if(varType == "BYTE"){
		if((uint8_t)req.value == req.value){
			AdsVariable<uint8_t> var {route, req.varName};
			var = (uint8_t)req.value;
			ROS_INFO("The %s %s now equals %u",varType.c_str(),req.varName.c_str(),(uint8_t)req.value);
		}else dataCorrect = false;
	  }else if(varType == "WORD" || varType == "UINT"){
		if((uint16_t)req.value == req.value){
			AdsVariable<uint16_t> var {route, req.varName};
			var = (uint16_t)req.value;
			ROS_INFO("The %s %s now equals %u",varType.c_str(),req.varName.c_str(),(uint16_t)req.value);
		}else dataCorrect = false;
	  }else if(varType == "INT"){
		if((int16_t)req.value == req.value){
			AdsVariable<int16_t> var {route, req.varName};
			var = (int16_t)req.value;
			ROS_INFO("The %s %s now equals %d",varType.c_str(),req.varName.c_str(),(int16_t)req.value);
		}else dataCorrect = false;
	  }else if(varType == "DWORD" || varType == "UDINT"){
		if((uint32_t)req.value == req.value){
			AdsVariable<uint32_t> var {route, req.varName};
			var = (uint32_t)req.value;
			ROS_INFO("The %s %s now equals %u",varType.c_str(),req.varName.c_str(),(uint32_t)req.value);
		}else dataCorrect = false;
	  }else if(varType == "DINT"){
		if((int32_t)req.value == req.value){
			AdsVariable<int32_t> var {route, req.varName};
			var = (int32_t)req.value;
			ROS_INFO("The %s %s now equals %d",varType.c_str(),req.varName.c_str(),(int32_t)req.value);
		}else dataCorrect = false;
	  }else if(varType == "LWORD" || varType == "ULINT"){
		if((uint64_t)req.value == req.value){
			AdsVariable<uint64_t> var {route, req.varName};
			var = (uint64_t)req.value;
			ROS_INFO("The %s %s now equals %lu",varType.c_str(),req.varName.c_str(),(uint64_t)req.value);
		}else dataCorrect = false;
	  }else if(varType == "LINT"){
		if((int64_t)req.value == req.value){
			AdsVariable<int64_t> var {route, req.varName};
			var = (int64_t)req.value;
			ROS_INFO("The %s %s now equals %ld",varType.c_str(),req.varName.c_str(),int64_t(req.value));
		}else dataCorrect = false;
	  }else if(varType == "REAL"){
		if((float)req.value/2 != (float)req.value){
			AdsVariable<float> var {route, req.varName};
			var = (float)req.value;
			ROS_INFO("The %s %s now equals %f",varType.c_str(),req.varName.c_str(),(float)req.value);
		}else dataCorrect = false;
	  }else if(varType == "LREAL"){
		AdsVariable<double> var {route, req.varName};
		var = req.value;
		ROS_INFO("The %s %s now equals %lf",varType.c_str(),req.varName.c_str(),req.value);
	  }else if(varType == "DATE"){
		if((uint32_t)req.value == req.value){
			AdsVariable<uint32_t> var {route, req.varName};
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
		}else dataCorrect = false;
	  }else if(varType == "TIME" || varType == "TIME_OF_DAY" || varType == "LTIME"){
		if((uint32_t)req.value == req.value){
			AdsVariable<uint32_t> var {route, req.varName};
			if(varType == "TIME" || varType == "TIME_OF_DAY"){
				var = (uint32_t)req.value;
			}else if(varType == "LTIME"){
				var = (uint32_t)(req.value * 1000000);
			}
			long lTime = (long)(req.value);
			long lMs = lTime % 1000;
			long lSec = (lTime/1000) % 60;
			long lMin = (lTime/60000) % 60;
			long lHeu = (lTime/3600000) % 24;
			ROS_INFO("%s set to %luh%lum%lus%lums",req.varName.c_str(),lHeu,lMin,lSec,lMs);
		}else dataCorrect = false;
	  }else {
		ROS_ERROR("Variable type not correct");
		return false;
	}

	if(!dataCorrect) {
		ROS_ERROR("Data size not correct");
		return false;
	}
	
	return true;
}

bool adsReadValue(beckhoff_plc_control::ADSReadValue::Request  &req, beckhoff_plc_control::ADSReadValue::Response &res, ros::NodeHandle* n){
	//------------------ Routing to remote PLC --------------------
	n->getParam("/RosAds_server/remoteNetId", remoteNetId_param);
	n->getParam("/RosAds_server/remoteIpV4", remoteIpV4_param);

	const AmsNetId remoteNetId {remoteNetId_param};
	char remoteIpV4[remoteIpV4_param.length()+1];
	strcpy(remoteIpV4, remoteIpV4_param.c_str());
	AdsDevice route {remoteIpV4, remoteNetId, AMSPORT_R0_PLC_TC3};
	//-------------------------------------------------------------

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
	  }else if(varType == "BYTE"){
		AdsVariable<uint8_t> var {route, req.varName};
		res.value = var;
		ROS_INFO("The %s %s equals %u",varType.c_str(),req.varName.c_str(),(uint8_t)res.value);
	  }else if(varType == "WORD" || varType == "UINT"){
		AdsVariable<uint16_t> var {route, req.varName};
		res.value = var;
		ROS_INFO("The %s %s equals %u",varType.c_str(),req.varName.c_str(),(uint16_t)res.value);
	  }else if(varType == "INT"){
		AdsVariable<int16_t> var {route, req.varName};
		res.value = var;
		ROS_INFO("The %s %s equals %d",varType.c_str(),req.varName.c_str(),(int16_t)res.value);
	  }else if(varType == "DWORD" || varType == "UDINT"){
		AdsVariable<uint32_t> var {route, req.varName};
		res.value = var;
		ROS_INFO("The %s %s equals %u",varType.c_str(),req.varName.c_str(),(uint32_t)res.value);
	  }else if(varType == "DINT"){
		AdsVariable<int32_t> var {route, req.varName};
		res.value = var;
		ROS_INFO("The %s %s equals %d",varType.c_str(),req.varName.c_str(),(int32_t)res.value);
	  }else if(varType == "LWORD" || varType == "ULINT"){
		AdsVariable<uint64_t> var {route, req.varName};
		res.value = var;
		ROS_INFO("The %s %s equals %lu",varType.c_str(),req.varName.c_str(),(uint64_t)res.value);
	  }else if(varType == "LINT"){
		AdsVariable<int64_t> var {route, req.varName};
		res.value = var;
		ROS_INFO("The %s %s equals %ld",varType.c_str(),req.varName.c_str(),int64_t(res.value));
	  }else if(varType == "REAL"){
		AdsVariable<float> var {route, req.varName};
		res.value = var;
		ROS_INFO("The %s %s equals %f",varType.c_str(),req.varName.c_str(),(float)res.value);
	  }else if(varType == "LREAL"){
		AdsVariable<double> var {route, req.varName};
		res.value = var;
		ROS_INFO("The %s %s equals %lf",varType.c_str(),req.varName.c_str(),res.value);
	  }else if(varType == "DATE"){
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
	  }else if(varType == "TIME" || varType == "TIME_OF_DAY" || varType == "LTIME"){
			AdsVariable<uint32_t> var {route, req.varName};
			res.value = var;
			long lTime = (long)(var);
			long lMs = lTime % 1000;
			long lSec = (lTime/1000) % 60;
			long lMin = (lTime/60000) % 60;
			long lHeu = (lTime/3600000) % 24;
			ROS_INFO("%s equals %luh%lum%lus%lums",req.varName.c_str(),lHeu,lMin,lSec,lMs);
	  }else {
		ROS_ERROR("Variable type not correct");
		return false;
	}
	
	return true;
}

bool adsReadVariables(beckhoff_plc_control::ADSReadVariables::Request  &req, beckhoff_plc_control::ADSReadVariables::Response &res)
{
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
  	return false;
  }
}

int main(int argc, char **argv)
{
	string localNetId_param;

	ros::init(argc, argv, "RosAds_server");
	ros::NodeHandle n;

	n.getParam("/RosAds_server/localNetId", localNetId_param);
	AdsSetLocalAddress(localNetId_param);

	ros::ServiceServer writingValueService = n.advertiseService<beckhoff_plc_control::ADSWriteValue::Request, beckhoff_plc_control::ADSWriteValue::Response>("ADS_write_value", boost::bind(&adsWriteValue, _1, _2, &n));
	ros::ServiceServer readingValueService = n.advertiseService<beckhoff_plc_control::ADSReadValue::Request, beckhoff_plc_control::ADSReadValue::Response>("ADS_read_value", boost::bind(&adsReadValue, _1, _2, &n));
	ros::ServiceServer readingVariablesService = n.advertiseService("ADS_read_variables", adsReadVariables);
	ROS_INFO("Ready to communicate with the remote PLC via ADS.");
	ros::spin();

	return 0;
}
