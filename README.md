# ESP32Logger
Logger for ESP32 for debugging C++-code

## Summary
ESP32Logger is a C++-library for ESP32/Arduino.
The library contains a class ESP32Logger wich contains the functionality.
The logging-functions are used via macro-functions; this makes it possible not only to deactivate logging but to compile the code even w/o any logging-code included, and without deleting your logging-comamands in the project.
Logging is done to serial-output.

## Outlook
Add other forms of output as acutally the serial-output, like e.g. via MQTT.

## Include the library in the project

Include library in the project and activate in general the logging-funcionality (i.e. include code for logging in the project):
```C++
#define ESP32DEBUGGING
#include <ESP32Logger.h>
```
Just comment out the #define ESP32DEBUGGING definition and no logging-code will be included to the project.
It is not necessary to remove the logging-commands out of the project-code.

## Inicialize logging with DBGINI



