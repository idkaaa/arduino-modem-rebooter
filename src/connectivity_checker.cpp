#include <WiFi.h>
#include <ESP32Ping.h>
#include "logger.cpp"

Logger logger = Logger();

IPAddress ip1 = ip1.fromString("4.2.2.1"); // The first remote ip to ping
IPAddress ip2 = ip2.fromString("8.8.8.8"); // The second remote ip to ping

// Load secrets from arduino_secrets.h
#include "arduino_secrets.h"
IPAddress ipGateway = ipGateway.fromString(GATEWAY_IP); // Gateway device IP to ping
const char *ssid = WIFI_SSID;                           // WiFi SSID
const char *password = WIFI_PASSWORD;                   // WiFi Password

/* Initialize the wifi connection*/
void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(300);
    }
    logger.logLine("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    logger.logLine(WiFi.localIP().toString().c_str());
}

/// @brief Check if the wifi connection is still active.
/// @details If the connection is not active, it will try to reconnect.
/// @return bool - true if the connection is active, false if it is not.
bool tryReconnectWiFi()
{
    logger.logLine("\nChecking wifi connection.");
    if (WiFi.status() != WL_CONNECTED)
    {
        logger.logLine("Reconnecting to WiFi...");
        WiFi.disconnect();
        return WiFi.reconnect();
    }
    return true;
}

/* Check if the gateway device can be reached.*/
bool isGatewayUp()
{
    Serial.print("\nPinging gateway IP: ");
    logger.logLine(GATEWAY_IP);
    if (Ping.ping(ipGateway, 1))
    {
        logger.logLine("Gateway is up.");
        return true;
    }
    logger.logLine("Gateway is down.");
    return false;
}

/* Check if the internet can be reached */
bool isInternetConnected()
{
    logger.logLine("Pinging internet IPs...");
    bool success = Ping.ping(ip1, 1) || Ping.ping(ip2, 1);
    if (success)
    {
        logger.logLine("Ping succesful.");
        return true;
    }
    logger.logLine("Ping failed");
    return false;
}