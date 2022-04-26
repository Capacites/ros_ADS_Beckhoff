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
    BOOL,
    UINT8_T,
    INT8_T,
    UINT16_T,
    INT16_T,
    UINT32_T,
    INT32_T,
    INT64_T,
    FLOAT,
    DOUBLE,
    DATE,
};

std::map<std::string, variant_t> decode(const ros_ads_msgs::ADS::ConstPtr &msg);

}

#endif
