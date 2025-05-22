#include <RCSwitch.h>
#include "logger.cpp"

#define TXD2 32
#define PULSE_LENGTH 414
#define PROTOCOL_NUMBER 1

/* Class to reboot a modem/router */
class ModemRebooter
{
    private:
        Logger logger = Logger();
        RCSwitch mySwitch = RCSwitch();
        /* power on the modem/router*/
        void powerOn()
        {
            logger.logLine("Sending On...");
            mySwitch.send("010101110111010011111001");
        }
        /* power off the modem/router*/
        void powerOff()
        {
            logger.logLine("Sending Off...");
            mySwitch.send("010101110111010011110001");
        }
    public:
        ModemRebooter()
        {
            pinMode(TXD2, OUTPUT);
            mySwitch.enableTransmit(TXD2);
            mySwitch.setProtocol(PROTOCOL_NUMBER);
            mySwitch.setPulseLength(PULSE_LENGTH);
            //mySwitch.setRepeatTransmit(10);
        }

        /* Reboot the internet modem/router (power off then on)*/
        void reboot()
        {
            powerOff();
            delay(3000);
            powerOn();
        }
        
};