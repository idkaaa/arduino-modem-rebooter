#include <Arduino.h>
#include <WiFi.h>
#include "connectivity_checker.cpp"
#include "modem_rebooter.cpp"
#include "logger.cpp"

Logger logger = Logger();
ModemRebooter modemRebooter = ModemRebooter();

unsigned long intervalWifiCheck = 5000;
unsigned long intervalModemRebooting = 30000;
unsigned long intervalInternetCheck = 5000;

void setup()
{
    initWiFi();
}

void loop()
{
    if (!tryReconnectWiFi())
    {
        delay(intervalWifiCheck);
        //don't try to check anythything else before wifi is back up
        return; 
        
    }

    //if modem could still be rebooting...
    if (!isGatewayUp())
    {
        delay(intervalModemRebooting);
        //don't try to check anything else before modem is back up
        return;
    }

    if (!isInternetConnected())
    {
        logger.logLine("Internet is NOT connected.");
        modemRebooter.reboot();
    }
    delay(intervalInternetCheck);
}