#include <Arduino.h>
#include <WiFi.h>
#include <ezButton.h>
#include <arduino-timer.h>

#include "utilities.h"
#include "logger.h"
#include "connectivity_checker.h"
#include "modem_rebooter.h"
// Load secrets from arduino_secrets.h
#include "arduino_secrets.h"


/* 
User defined variables/constants 
*/
#define MODEM_REBOOT_PIN 23
#define BUTTON_PIN 5
#define LED_CONNECTED_PIN 33
// cooldown to wait before checking if the modem is back up after rebooting
unsigned long delayAfterModemReboot = 60000; 
unsigned long intervalInternetCheck = 5000;
IPAddress ip1 = CreateIpAddressFromString("4.2.2.1"); // The first remote ip to ping
IPAddress ip2 = CreateIpAddressFromString("8.8.8.8"); // The second remote ip to ping
IPAddress ipGateway = CreateIpAddressFromString(GATEWAY_IP); // Gateway device IP to ping

/*
program variables
*/
auto timer = timer_create_default();
Timer<>::Task internetCheckTask;

ezButton button(BUTTON_PIN);
Logger logger;
ModemRebooter modemRebooter(MODEM_REBOOT_PIN);
ConnectivityChecker connectivityChecker = ConnectivityChecker(
        ip1, 
        ip2,
        ipGateway, 
        WIFI_SSID, 
        WIFI_PASSWORD
    );
bool shouldEnableModemRebooting = true; //should any checking be done?
unsigned long nextRebootAt = 0;

void OnInternetCheckFail()
{
    logger.logLine("Internet is NOT connected.");
    if (!shouldEnableModemRebooting)
    {
        logger.logLine("Modem rebooting is disabled, not rebooting modem.");
        return;
    }
    if (millis() < nextRebootAt)
    {
        logger.logLine("Internet is NOT connected, waiting for cooldown period to pass before rebooting modem again.");
        return;
    }
    logger.logLine("Rebooting modem...");
    nextRebootAt = millis() + delayAfterModemReboot + intervalInternetCheck;
    modemRebooter.reboot();
}

void OnInternetCheckSuccess()
{
    logger.logLine("Internet is connected.");
}

bool CheckInternet(void *)
{
    unsigned long currentTime = millis();
    logger.logLine("CurrentTime: " + String(currentTime));
    if (!connectivityChecker.tryReconnectWiFi())
    {
        //don't try to check anythything else before wifi is back up
        return true; 
    }
    if (!connectivityChecker.isInternetConnected())
    {
        OnInternetCheckFail();
    }
    else
    {
        OnInternetCheckSuccess();
    }
    return true;
}

void UpdateDisplay()
{
    if (shouldEnableModemRebooting)
    {
        if (digitalRead(LED_CONNECTED_PIN) == LOW)
        {
            logger.logLine("Turning on LED.");
            digitalWrite(LED_CONNECTED_PIN, HIGH);
        }
    }
    else
    {
        if (digitalRead(LED_CONNECTED_PIN) == HIGH)
        {
            logger.logLine("Turning off LED.");
            digitalWrite(LED_CONNECTED_PIN, LOW);
        }
    }
}

void OnButtonPress()
{
    logger.logLine("Button pressed.");
    if (shouldEnableModemRebooting)
    {
        logger.logLine("Disabling modem rebooting.");
        shouldEnableModemRebooting = false;
    }
    else
    {
        logger.logLine("Enabling modem rebooting.");
        shouldEnableModemRebooting = true;
    }
}

void setup()
{
    connectivityChecker.initWiFi();
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    button.setDebounceTime(100);
    pinMode(LED_CONNECTED_PIN, OUTPUT);
    internetCheckTask = timer.every(intervalInternetCheck, CheckInternet);
}

void loop()
{
    button.loop();
    if (button.isPressed())
    {
        OnButtonPress();
    }
    timer.tick();
    UpdateDisplay();
}