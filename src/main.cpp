#include <Arduino.h>
#include <WiFi.h>
#include <ESP32Ping.h>

// Load secrets from arduino_secrets.h
#include "arduino_secrets.h"
IPAddress ipGateway = ipGateway.fromString(GATEWAY_IP); // Gateway device IP to ping
const char *ssid = WIFI_SSID;                           // WiFi SSID
const char *password = WIFI_PASSWORD;                   // WiFi Password

// Set other constants
unsigned long intervalWifiCheck = 5000;
unsigned long intervalModemRebooting = 30000;
unsigned long intervalInternetCheck = 5000;
IPAddress ip1 = ip1.fromString("4.2.2.1"); // The first remote ip to ping
IPAddress ip2 = ip2.fromString("8.8.8.8"); // The second remote ip to ping


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
    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

bool isGatewayUp()
{
    Serial.print("\nPinging gateway IP: ");
    Serial.println(GATEWAY_IP);
    if (Ping.ping(ipGateway, 1))
    {
        Serial.println("Gateway is up.");
        return true;
    }
    Serial.println("Gateway is down.");
    return false;
}

bool isInternetConnected()
{
    Serial.println("Pinging internet IPs...");
    bool success = Ping.ping(ip1, 1) || Ping.ping(ip2, 1);
    if (success)
    {
        Serial.println("Ping succesful.");
        return true;
    }
    Serial.println("Ping failed");
    return false;
}

void setup()
{
    Serial.begin(115200);
    delay(1000);
    initWiFi();
}

void loop()
{
    Serial.println("\nChecking wifi connection.");
    if (WiFi.status() != WL_CONNECTED)
    {
        Serial.println("Reconnecting to WiFi...");
        WiFi.disconnect();
        WiFi.reconnect();
        delay(intervalWifiCheck);
        return;
    }

    //if rebooting modem...
    if (!isGatewayUp())
    {
        delay(intervalModemRebooting);
        return;
    }

    if (isInternetConnected())
    {
        Serial.println("Internet is connected.");
    }
    else
    {
        Serial.println("Internet is NOT connected.");
    }
    delay(intervalInternetCheck);
}