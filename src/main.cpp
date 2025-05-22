#include <Arduino.h>
#include <WiFi.h>

#include "logger.h"
#include "connectivity_checker.h"
#include "modem_rebooter.h"
// Load secrets from arduino_secrets.h
#include "arduino_secrets.h"

Logger logger = Logger();
ModemRebooter modemRebooter = ModemRebooter();
IPAddress ip1 = ip1.fromString("4.2.2.1"); // The first remote ip to ping
IPAddress ip2 = ip2.fromString("8.8.8.8"); // The second remote ip to ping
IPAddress ipGateway = ipGateway.fromString(GATEWAY_IP); // Gateway device IP to ping
ConnectivityChecker connectivityChecker = ConnectivityChecker(ip1, ip2, ipGateway, WIFI_SSID, WIFI_PASSWORD);

unsigned long intervalWifiCheck = 5000;
unsigned long intervalModemRebooting = 30000;
unsigned long intervalInternetCheck = 5000;

void setup()
{
    Serial.begin(115200);
    delay(1000);
    connectivityChecker.initWiFi();
}

void loop()
{
    if (!connectivityChecker.tryReconnectWiFi())
    {
        delay(intervalWifiCheck);
        //don't try to check anythything else before wifi is back up
        return; 
    }

    //if modem could still be rebooting...
    if (! connectivityChecker.isGatewayUp())
    {
        delay(intervalModemRebooting);
        //don't try to check anything else before modem is back up
        return;
    }

    if (!connectivityChecker.isInternetConnected())
    {
        logger.logLine("Internet is NOT connected.");
        modemRebooter.reboot();
    }
    delay(intervalInternetCheck);
}