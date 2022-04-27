#ifndef HEADER_H_ADS_MSGS
#define HEADER_H_ADS_MSGS

#include <map>
#include <string>
#include <variant>
#include <algorithm>
#include <time.h>
#include <ros_ads_msgs/ADS.h>

namespace ros_ads_msgs {

using variant_t = std::variant<bool, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, int64_t, float, double, tm>;

enum{
    BOOL = 0,
    UINT8_T = 1,
    INT8_T = 2,
    UINT16_T = 3,
    INT16_T = 4,
    UINT32_T = 5,
    INT32_T = 6,
    INT64_T = 7,
    FLOAT = 8,
    DOUBLE = 9,
    DATE = 10,
};

std::map<std::string, variant_t> decode(const ros_ads_msgs::ADS::ConstPtr &msg);

}

#endif
