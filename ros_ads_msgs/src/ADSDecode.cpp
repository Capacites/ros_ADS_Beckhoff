#include <ros_ads_msgs/ADSDecode.h>

using namespace ros_ads_msgs;

std::map<std::string, variant_t> ros_ads_msgs::decode(const ros_ads_msgs::ADS::ConstPtr &msg)
{
    std::map<std::string, std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm>> result;
    std::string name;
    std::string type;
    double value;
    bool cresult;
    std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm> converted_value;

    int minSize = std::min(std::min(msg->varNames.size(), msg->varTypes.size()), msg->varValues.size());

    for(int index = 0; index < minSize; index ++)
    {
        cresult = false;
        name = msg->varNames[index];
        type = msg->varTypes[index];
        value = msg->varValues[index];
        do
        {
            if(type == "BOOL")
            {
                converted_value = static_cast<bool>(value);
                cresult = true;
            }
            if(type == "BYTE" || type == "USINT")
            {
                converted_value = static_cast<uint8_t>(value);
                cresult = true;
            }
            if(type == "SINT")
            {
                converted_value = static_cast<int8_t>(value);
                cresult = true;
            }
            if(type == "WORD" || type == "UINT")
            {
                converted_value = static_cast<uint16_t>(value);
                cresult = true;
            }
            if(type == "INT")
            {
                converted_value = static_cast<int16_t>(value);
                cresult = true;
            }
            if(type == "DWORD" || type == "UDINT")
            {
                converted_value = static_cast<uint32_t>(value);
                cresult = true;
            }
            if(type == "DINT")
            {
                converted_value = static_cast<int32_t>(value);
                cresult = true;
                break;
            }
            if(type == "LINT")
            {
                converted_value = static_cast<int64_t>(value);
                cresult = true;
                break;
            }
            if(type == "REAL")
            {
                converted_value = static_cast<float>(value);
                cresult = true;
                break;
            }
            if(type == "LREAL")
            {
                converted_value = static_cast<double>(value);
                cresult = true;
                break;
            }
            if(type == "DATE")
            {
                auto temp = static_cast<uint32_t>(value);
                ros::Time currentDate(temp);
                time_t tDate(currentDate.toSec());
                tm tmDate;
                gmtime_r(&tDate,&tmDate);
                converted_value = tmDate;
                cresult = true;
                break;
            }
        }
        while(false);
        if(cresult)
        {
            result[name] = converted_value;
        }
    }
    return result;
}

