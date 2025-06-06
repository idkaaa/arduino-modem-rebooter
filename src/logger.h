#ifndef Logger_h
#define Logger_h
#include <Arduino.h>

/* class to log to serial/screen/whatever...*/
class Logger {
    private:
    public:
        Logger() {
            Serial.begin(115200);
            delay(1000);
        }

        /* log on the same line of output */
        void log(String message) {
            Serial.print(message);
        }

        /* log on a new line of output */
        void logLine(String message) {
            Serial.println(message);
        }
};

#endif