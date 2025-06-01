#include <Arduino.h>
#include <WiFi.h>
#include <ezButton.h>
#include <arduino-timer.h>

#include "utilities.h"
#include "logger.h"
#include "connectivity_checker.h"
#include "modem_rebooter.h"
#include "alert_sounder.h"
// Load secrets from arduino_secrets.h
#include "arduino_secrets.h"


/* 
User defined variables/constants 
*/
#define MODEM_REBOOT_PIN 23
#define BUTTON_PIN 5
#define LED_ONLINE_PIN 27
#define LED_OFFLINE_PIN 26
#define BUZZER_PIN 17
// cooldown to wait before checking if the modem is back up after rebooting
unsigned long delayAfterModemReboot = 80000; 
unsigned long intervalInternetCheck = 5000;
IPAddress ip1 = CreateIpAddressFromString("4.2.2.1"); // The first remote ip to ping
IPAddress ip2 = CreateIpAddressFromString("8.8.8.8"); // The second remote ip to ping
IPAddress ipGateway = CreateIpAddressFromString(GATEWAY_IP); // Gateway device IP to ping

/*
program variables
*/
auto timer = timer_create_default();
Timer<>::Task internetCheckTask;

AlertSounder* backOnlineSounder = new NeverGonnaGiveYouUpSounder(BUZZER_PIN);
AlertSounder* offlineSounder = new SirenSounder(BUZZER_PIN);
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

unsigned long rebootCount = 0; //how many times the modem has been rebooted
bool shouldEnableModemRebooting = true; //should any checking be done?
unsigned long nextRebootAt = 0;
bool isModemRebooting = false;
bool isOnline = false; //is the internet connection online?

void OnInternetCheckFail()
{
    logger.logLine("Internet is NOT connected.");
    isOnline = false;
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
    isModemRebooting = true;
    rebootCount++;
    offlineSounder->play(); // Play the alert sound
}

void OnInternetCheckSuccess()
{
    logger.logLine("Internet is connected.");
    isOnline = true;
    if (isModemRebooting)
    {
        logger.logLine("Modem rebooting completed, back online.");
        isModemRebooting = false;
        backOnlineSounder->play(); // Play the alert sound
    }
}

bool CheckInternet(void *)
{
    unsigned long currentTime = millis();
    logger.logLine(
        "\nCurrentTime: " + String(currentTime) + " Reboot Count: " + String(rebootCount));
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

/// @brief Switch led on or off based on the desired state.
/// @param ledPin 
/// @param desiredState 
void ToggleLed(int ledPin, int desiredState)
{
    int state = digitalRead(ledPin);
    if (state == desiredState)
    {
        return; // no change needed
    }
    digitalWrite(ledPin, desiredState);
}

void UpdateDisplay()
{
    if (!shouldEnableModemRebooting)
    {
        //disable green/red leds
        ToggleLed(LED_ONLINE_PIN, LOW);
        ToggleLed(LED_OFFLINE_PIN, LOW);
        return;
    }
    
    if (isOnline)
    {
        ToggleLed(LED_ONLINE_PIN, HIGH);
    
        ToggleLed(LED_OFFLINE_PIN, LOW);
    }
    else
    {
        ToggleLed(LED_ONLINE_PIN, LOW);
        ToggleLed(LED_OFFLINE_PIN, HIGH);
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
    pinMode(LED_ONLINE_PIN, OUTPUT);
    pinMode(LED_OFFLINE_PIN, OUTPUT);
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