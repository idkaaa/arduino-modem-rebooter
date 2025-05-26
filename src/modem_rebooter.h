#include <RCSwitch.h>
#include "logger.h"

#define PULSE_LENGTH 414
#define PROTOCOL_NUMBER 1

/* Class to reboot a modem/router */
class ModemRebooter : Logger
{
    private:
        RCSwitch mySwitch = RCSwitch();
        /* power on the modem/router*/
        void powerOn()
        {
            logLine("Powering modem on...");
            mySwitch.send("010101110111010011111001");
        }
        /* power off the modem/router*/
        void powerOff()
        {
            logLine("Powering modem off...");
            mySwitch.send("010101110111010011110001");
        }
    public:
        /// @brief Constructor for the ModemRebooter class.
        /// @param txPin remote control pin to use.
        ModemRebooter(int txPin)
        {
            pinMode(txPin, OUTPUT);
            mySwitch.enableTransmit(txPin);
            mySwitch.setProtocol(PROTOCOL_NUMBER);
            mySwitch.setPulseLength(PULSE_LENGTH);
            //mySwitch.setRepeatTransmit(10);
        }

        /* Reboot the internet modem/router (power off then on)*/
        void reboot()
        {
            powerOff();
            delay(2000);
            powerOn();
        }
        
};