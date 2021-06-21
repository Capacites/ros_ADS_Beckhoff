#include "ros/ros.h"
#include "ros/package.h"
#include "beckhoff_plc_control/ADSWriteValue.h"
#include "beckhoff_plc_control/ADSReadValue.h"

int main(int argc, char **argv)
{
  ros::init(argc, argv, "Client_test_node");
  ros::NodeHandle n;
  ros::ServiceClient writingClient;
  ros::ServiceClient readingClient;

  writingClient = n.serviceClient<beckhoff_plc_control::ADSWriteValue>("ADS_write_value");
  readingClient = n.serviceClient<beckhoff_plc_control::ADSReadValue>("ADS_read_value");

  //BOOL test
  beckhoff_plc_control::ADSWriteValue adsWriteBOOLTestSrv;
  adsWriteBOOLTestSrv.request.varName = "ROS.TEST_BOOL";
  adsWriteBOOLTestSrv.request.value = 1;
  if(writingClient.call(adsWriteBOOLTestSrv)){
	ROS_INFO("Wrote %u in %s", (uint8_t)adsWriteBOOLTestSrv.request.value, adsWriteBOOLTestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadBOOLTestSrv;
  	adsReadBOOLTestSrv.request.varName = "ROS.TEST_BOOL";
	if(readingClient.call(adsReadBOOLTestSrv)){
		ROS_INFO("Read %u in %s", (uint8_t)adsReadBOOLTestSrv.response.value, adsReadBOOLTestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadBOOLTestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteBOOLTestSrv.request.varName.c_str());
  }

  //BYTE test
  beckhoff_plc_control::ADSWriteValue adsWriteBYTETestSrv;
  adsWriteBYTETestSrv.request.varName = "ROS.TEST_BYTE";
  adsWriteBYTETestSrv.request.value = 255;
  if(writingClient.call(adsWriteBYTETestSrv)){
	ROS_INFO("Wrote %u in %s", (uint8_t)adsWriteBYTETestSrv.request.value, adsWriteBYTETestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadBYTETestSrv;
  	adsReadBYTETestSrv.request.varName = "ROS.TEST_BYTE";
	if(readingClient.call(adsReadBYTETestSrv)){
		ROS_INFO("Read %u in %s", (uint8_t)adsReadBYTETestSrv.response.value, adsReadBYTETestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadBYTETestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteBYTETestSrv.request.varName.c_str());
  }
  
  //WORD test
  beckhoff_plc_control::ADSWriteValue adsWriteWORDTestSrv;
  adsWriteWORDTestSrv.request.varName = "ROS.TEST_WORD";
  adsWriteWORDTestSrv.request.value = 65535;
  if(writingClient.call(adsWriteWORDTestSrv)){
	ROS_INFO("Wrote %u in %s", (uint16_t)adsWriteWORDTestSrv.request.value, adsWriteWORDTestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadWORDTestSrv;
  	adsReadWORDTestSrv.request.varName = "ROS.TEST_WORD";
	if(readingClient.call(adsReadWORDTestSrv)){
		ROS_INFO("Read %u in %s", (uint16_t)adsReadWORDTestSrv.response.value, adsReadWORDTestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadWORDTestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteWORDTestSrv.request.varName.c_str());
  }

  //UINT test
  beckhoff_plc_control::ADSWriteValue adsWriteUINTTestSrv;
  adsWriteUINTTestSrv.request.varName = "ROS.TEST_UINT";
  adsWriteUINTTestSrv.request.value = 65535;
  if(writingClient.call(adsWriteUINTTestSrv)){
	ROS_INFO("Wrote %u in %s", (uint16_t)adsWriteUINTTestSrv.request.value, adsWriteUINTTestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadUINTTestSrv;
  	adsReadUINTTestSrv.request.varName = "ROS.TEST_UINT";
	if(readingClient.call(adsReadUINTTestSrv)){
		ROS_INFO("Read %u in %s", (uint16_t)adsReadUINTTestSrv.response.value, adsReadUINTTestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadUINTTestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteUINTTestSrv.request.varName.c_str());
  }

  //INT test
  beckhoff_plc_control::ADSWriteValue adsWriteINTTestSrv;
  adsWriteINTTestSrv.request.varName = "ROS.TEST_INT";
  adsWriteINTTestSrv.request.value = -32768;
  if(writingClient.call(adsWriteINTTestSrv)){
	ROS_INFO("Wrote %d in %s", (int16_t)adsWriteINTTestSrv.request.value, adsWriteINTTestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadINTTestSrv;
  	adsReadINTTestSrv.request.varName = "ROS.TEST_INT";
	if(readingClient.call(adsReadINTTestSrv)){
		ROS_INFO("Read %d in %s", (int16_t)adsReadINTTestSrv.response.value, adsReadINTTestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadINTTestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteINTTestSrv.request.varName.c_str());
  }

  //DWORD test
  beckhoff_plc_control::ADSWriteValue adsWriteDWORDTestSrv;
  adsWriteDWORDTestSrv.request.varName = "ROS.TEST_DWORD";
  adsWriteDWORDTestSrv.request.value = 65535;
  if(writingClient.call(adsWriteDWORDTestSrv)){
	ROS_INFO("Wrote %u in %s", (uint32_t)adsWriteDWORDTestSrv.request.value, adsWriteDWORDTestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadDWORDTestSrv;
  	adsReadDWORDTestSrv.request.varName = "ROS.TEST_DWORD";
	if(readingClient.call(adsReadDWORDTestSrv)){
		ROS_INFO("Read %u in %s", (uint32_t)adsReadDWORDTestSrv.response.value, adsReadDWORDTestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadDWORDTestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteDWORDTestSrv.request.varName.c_str());
  }

  //UDINT test
  beckhoff_plc_control::ADSWriteValue adsWriteUDINTTestSrv;
  adsWriteUDINTTestSrv.request.varName = "ROS.TEST_UDINT";
  adsWriteUDINTTestSrv.request.value = 10;
  if(writingClient.call(adsWriteUDINTTestSrv)){
	ROS_INFO("Wrote %u in %s", (uint32_t)adsWriteUDINTTestSrv.request.value, adsWriteUDINTTestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadUDINTTestSrv;
  	adsReadUDINTTestSrv.request.varName = "ROS.TEST_UDINT";
	if(readingClient.call(adsReadUDINTTestSrv)){
		ROS_INFO("Read %u in %s", (uint32_t)adsReadUDINTTestSrv.response.value, adsReadUDINTTestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadUDINTTestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteUDINTTestSrv.request.varName.c_str());
  }

  //DINT test
  beckhoff_plc_control::ADSWriteValue adsWriteDINTTestSrv;
  adsWriteDINTTestSrv.request.varName = "ROS.TEST_DINT";
  adsWriteDINTTestSrv.request.value = -10;
  if(writingClient.call(adsWriteDINTTestSrv)){
	ROS_INFO("Wrote %d in %s", (int32_t)adsWriteDINTTestSrv.request.value, adsWriteDINTTestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadDINTTestSrv;
  	adsReadDINTTestSrv.request.varName = "ROS.TEST_DINT";
	if(readingClient.call(adsReadDINTTestSrv)){
		ROS_INFO("Read %d in %s", (int32_t)adsReadDINTTestSrv.response.value, adsReadDINTTestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadDINTTestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteDINTTestSrv.request.varName.c_str());
  }

  //LWORD test
  beckhoff_plc_control::ADSWriteValue adsWriteLWORDTestSrv;
  adsWriteLWORDTestSrv.request.varName = "ROS.TEST_LWORD";
  adsWriteLWORDTestSrv.request.value = 10;
  if(writingClient.call(adsWriteLWORDTestSrv)){
	ROS_INFO("Wrote %lu in %s", (uint64_t)adsWriteLWORDTestSrv.request.value, adsWriteLWORDTestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadLWORDTestSrv;
  	adsReadLWORDTestSrv.request.varName = "ROS.TEST_LWORD";
	if(readingClient.call(adsReadLWORDTestSrv)){
		ROS_INFO("Read %lu in %s", (uint64_t)adsReadLWORDTestSrv.response.value, adsReadLWORDTestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadLWORDTestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteLWORDTestSrv.request.varName.c_str());
  }

  //ULINT test
  beckhoff_plc_control::ADSWriteValue adsWriteULINTTestSrv;
  adsWriteULINTTestSrv.request.varName = "ROS.TEST_ULINT";
  adsWriteULINTTestSrv.request.value = 10;
  if(writingClient.call(adsWriteULINTTestSrv)){
	ROS_INFO("Wrote %lu in %s", (uint64_t)adsWriteULINTTestSrv.request.value, adsWriteULINTTestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadULINTTestSrv;
  	adsReadULINTTestSrv.request.varName = "ROS.TEST_ULINT";
	if(readingClient.call(adsReadULINTTestSrv)){
		ROS_INFO("Read %lu in %s", (uint64_t)adsReadULINTTestSrv.response.value, adsReadULINTTestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadULINTTestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteULINTTestSrv.request.varName.c_str());
  }

  //LINT test
  beckhoff_plc_control::ADSWriteValue adsWriteLINTTestSrv;
  adsWriteLINTTestSrv.request.varName = "ROS.TEST_LINT";
  adsWriteLINTTestSrv.request.value = -10;
  if(writingClient.call(adsWriteLINTTestSrv)){
	ROS_INFO("Wrote %ld in %s", (int64_t)adsWriteLINTTestSrv.request.value, adsWriteLINTTestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadLINTTestSrv;
  	adsReadLINTTestSrv.request.varName = "ROS.TEST_LINT";
	if(readingClient.call(adsReadLINTTestSrv)){
		ROS_INFO("Read %ld in %s", (int64_t)adsReadLINTTestSrv.response.value, adsReadLINTTestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadLINTTestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteLINTTestSrv.request.varName.c_str());
  }

  //REAL test
  beckhoff_plc_control::ADSWriteValue adsWriteREALTestSrv;
  adsWriteREALTestSrv.request.varName = "ROS.TEST_REAL";
  adsWriteREALTestSrv.request.value = 10.101;
  if(writingClient.call(adsWriteREALTestSrv)){
	ROS_INFO("Wrote %f in %s", (float)adsWriteREALTestSrv.request.value, adsWriteREALTestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadREALTestSrv;
  	adsReadREALTestSrv.request.varName = "ROS.TEST_REAL";
	if(readingClient.call(adsReadREALTestSrv)){
		ROS_INFO("Read %f in %s", (float)adsReadREALTestSrv.response.value, adsReadREALTestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadREALTestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteREALTestSrv.request.varName.c_str());
  }

  //LREAL test
  beckhoff_plc_control::ADSWriteValue adsWriteLREALTestSrv;
  adsWriteLREALTestSrv.request.varName = "ROS.TEST_LREAL";
  adsWriteLREALTestSrv.request.value = 10.101;
  if(writingClient.call(adsWriteLREALTestSrv)){
	ROS_INFO("Wrote %lf in %s", (double)adsWriteLREALTestSrv.request.value, adsWriteLREALTestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadLREALTestSrv;
  	adsReadLREALTestSrv.request.varName = "ROS.TEST_LREAL";
	if(readingClient.call(adsReadLREALTestSrv)){
		ROS_INFO("Read %lf in %s", (double)adsReadLREALTestSrv.response.value, adsReadLREALTestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadLREALTestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteLREALTestSrv.request.varName.c_str());
  }

  //DATE test
  beckhoff_plc_control::ADSWriteValue adsWriteDATETestSrv;
  adsWriteDATETestSrv.request.varName = "ROS.TEST_DATE";
  adsWriteDATETestSrv.request.value = 1620206633;
  if(writingClient.call(adsWriteDATETestSrv)){
	ros::Time currentDate(adsWriteDATETestSrv.request.value);
	time_t tDate(currentDate.toSec());
	tm tmDate; 
	gmtime_r(&tDate,&tmDate);
	ROS_INFO("Wrote %d/%d/%d %d:%d:%d to %s",
		tmDate.tm_mday,
		tmDate.tm_mon + 1,
		tmDate.tm_year + 1900,
		tmDate.tm_hour,
		tmDate.tm_min,
		tmDate.tm_sec,
		adsWriteDATETestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadDATETestSrv;
  	adsReadDATETestSrv.request.varName = "ROS.TEST_DATE";
	if(readingClient.call(adsReadDATETestSrv)){
		ros::Time currentDate(adsReadDATETestSrv.response.value);
		time_t tDate(currentDate.toSec());
		tm tmDate; 
		gmtime_r(&tDate,&tmDate);
		ROS_INFO("Read %d/%d/%d %d:%d:%d in %s",
			tmDate.tm_mday,
			tmDate.tm_mon + 1,
			tmDate.tm_year + 1900,
			tmDate.tm_hour,
			tmDate.tm_min,
			tmDate.tm_sec,
			adsReadDATETestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadDATETestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteDATETestSrv.request.varName.c_str());
  }

  //TIME test
  beckhoff_plc_control::ADSWriteValue adsWriteTIMETestSrv;
  adsWriteTIMETestSrv.request.varName = "ROS.TEST_TIME";
  adsWriteTIMETestSrv.request.value = 100000;
  if(writingClient.call(adsWriteTIMETestSrv)){
	ros::Duration timer(adsWriteTIMETestSrv.request.value/1000);
	long lTime = (long)(timer.toSec()*1000);
	long lMs = lTime % 1000;
	long lSec = (lTime/1000) % 60;
	long lMin = (lTime/60000) % 60;
	long lHeu = (lTime/3600000) % 24;
	ROS_INFO("Wrote %luh%lum%lus%lums to %s",lHeu,lMin,lSec,lMs,adsWriteTIMETestSrv.request.varName.c_str());
	beckhoff_plc_control::ADSReadValue adsReadTIMETestSrv;
  	adsReadTIMETestSrv.request.varName = "ROS.TEST_TIME";
	if(readingClient.call(adsReadTIMETestSrv)){
		ros::Duration timer(adsReadTIMETestSrv.response.value/1000);
		long lTime = (long)(timer.toSec()*1000);
		long lMs = lTime % 1000;
		long lSec = (lTime/1000) % 60;
		long lMin = (lTime/60000) % 60;
		long lHeu = (lTime/3600000) % 24;
		ROS_INFO("Read %luh%lum%lus%lums in %s",lHeu,lMin,lSec,lMs,adsReadTIMETestSrv.request.varName.c_str());
	}else{
		ROS_INFO("Couldn't read in %s", adsReadTIMETestSrv.request.varName.c_str());
	}
  }else{
	ROS_INFO("Couldn't write in %s", adsWriteTIMETestSrv.request.varName.c_str());
  }

  return 0;
}
