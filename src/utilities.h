#include <Arduino.h>

/// @brief Create an IPAddress object from a string.
/// @param ipString 
/// @return IPAddress object
IPAddress CreateIpAddressFromString(String ipString)
{
    IPAddress ip;
    ip.fromString(ipString);
    return ip;
}