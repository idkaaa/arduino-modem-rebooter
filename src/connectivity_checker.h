#include <WiFi.h>
#include <ESP32Ping.h>
#include "logger.h"

/// @brief Class to check the connectivity of the ESP32 device.
class ConnectivityChecker : public Logger
{

    private:
        IPAddress ip1;// = ip1.fromString("4.2.2.1"); // The first remote ip to ping
        IPAddress ip2;// = ip2.fromString("8.8.8.8"); // The second remote ip to ping
        IPAddress ipGateway;// = ipGateway.fromString(GATEWAY_IP); // Gateway device IP to ping
        String ssid; // = WIFI_SSID;                           // WiFi SSID
        String password; // = WIFI_PASSWORD;                   // WiFi Password
    public:
        /// @brief Constructor for the ConnectivityChecker class.
        /// @param ip1 The first remote IP to ping.
        /// @param ip2 The second remote IP to ping.
        /// @param ipGateway the gateway device IP to ping.
        /// @param ssid the WiFi SSID.
        /// @param password the WiFi password.
        ConnectivityChecker(IPAddress ip1, IPAddress ip2, IPAddress ipGateway, String ssid, String password)
        {
            this->ip1 = ip1;
            this->ip2 = ip2;
            this->ipGateway = ipGateway;
            this->ssid = ssid;
            this->password = password;
            logLine("ConnectivityChecker initialized.");
        }

        /// @brief Initialize the wifi connection.
        /// @details This function will connect to the wifi network using the SSID and password defined in arduino_secrets.h.
    /* Initialize the wifi connection*/
        void initWiFi()
        {
            WiFi.mode(WIFI_STA);
            WiFi.begin(ssid, password);
            log("Connecting to WiFi ..");
            while (WiFi.status() != WL_CONNECTED)
            {
                log(".");
                delay(300);
            }
            logLine("\nConnected to the WiFi network");
            log("Local ESP32 IP: ");
            logLine(WiFi.localIP().toString());
        }

        /// @brief Check if the wifi connection is still active.
        /// @details If the connection is not active, it will try to reconnect.
        /// @return bool - true if the connection is active, false if it is not.
        bool tryReconnectWiFi()
        {
            logLine("Checking wifi connection.");
            if (WiFi.status() != WL_CONNECTED)
            {
                logLine("Reconnecting to WiFi...");
                WiFi.disconnect();
                return WiFi.reconnect();
            }
            return true;
        }

        /* Check if the gateway device can be reached.*/
        bool isGatewayUp()
        {
            logLine("Pinging gateway IP: " + ipGateway.toString() + " ...");
            if (Ping.ping(ipGateway, 1))
            {
                logLine("Gateway is up.");
                return true;
            }
            logLine("Gateway is down.");
            return false;
        }

        /* Check if the internet can be reached */
        bool isInternetConnected()
        {
            logLine("Pinging IP1: " + ip1.toString() + " and IP2: " + ip2.toString() + " ...");
            bool success = Ping.ping(ip1, 1) || Ping.ping(ip2, 1);
            if (success)
            {
                logLine("Ping succesful.");
                return true;
            }
            logLine("Ping failed");
            return false;
        }
};