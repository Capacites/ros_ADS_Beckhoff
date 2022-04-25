#include <ros_ads_msgs/ADSDecode.h>
using namespace  Ros_ads_msgs;

std::map<std::string, std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm>> decode(ros_ads_msgs::ADS msg)
{
    std::map<std::string, std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm>> result;
    std::string name;
    int type;
    double value;
    bool cresult;
    std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm> converted_value;

    int minSize = std::min(std::min(msg.varNames.size(), msg.varTypes.size()), msg.varValues.size());

    for(int index = 0; index < minSize; index ++)
    {
        cresult = false;
        name = msg.varNames[index];
        type = msg.varTypes[index];
        value = msg.varValues[index];
        switch (type)
        {
            case BOOL:
            {
                converted_value = static_cast<bool>(value);
                cresult = true;
            }
            case UINT8_T:
            {
                converted_value = static_cast<uint8_t>(value);
                cresult = true;
            }
            case INT8_T:
            {
                converted_value = static_cast<int8_t>(value);
                cresult = true;
            }
            case UINT16_T:
            {
                converted_value = static_cast<uint16_t>(value);
                cresult = true;
            }
            case INT16_T:
            {
                converted_value = static_cast<int16_t>(value);
                cresult = true;
            }
            case UINT32_T:
            {
                converted_value = static_cast<uint32_t>(value);
                cresult = true;
            }
            case INT32_T:
            {
                converted_value = static_cast<int32_t>(value);
                cresult = true;
                break;
            }
            case INT64_T:
            {
                converted_value = static_cast<int64_t>(value);
                cresult = true;
                break;
            }
            case FLOAT:
            {
                converted_value = static_cast<float>(value);
                cresult = true;
                break;
            }
            case DOUBLE:
            {
                converted_value = static_cast<double>(value);
                cresult = true;
                break;
            }
            case DATE:
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
            default:
            {

            }
        }
        if(cresult)
        {
            result[name] = converted_value;
        }
    }
    return result;
}

