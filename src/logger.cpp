#include <Arduino.h>

class Logger {
    private:
    public:
        Logger() {
            Serial.begin(115200);
            delay(1000);
        }

        /* log on the same line of output */
        void log(const char* message) {
            Serial.print(message);
        }

        /* log on a new line of output */
        void logLine(const char* message) {
            Serial.println(message);
        }
};