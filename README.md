# ESP32Logger
Logger for ESP32 for debugging C++-code

## Introduction
ESP32Logger is a C++-library for ESP32/Arduino.
The library contains a class ESP32Logger wich contains the functionality.
The logging-functions are used via macro-functions; this makes it possible not only to deactivate logging but to compile the code even w/o any logging-code included, and without deleting your logging-comamands in the project.

Logging is done to serial-output.

The logging includes 
* a timestamp (optional, can be time passed since start (in ms) or a specific time)
* name of the function which contains the logging-function.

Example code:
```C++
char s[] = "Test";
DBGLOG(Info, "This is an info-example; string-value s: %s.", s)
uint32_t u = 123;
DBGLOG(Error, "This is an error message; uint-value u: %u.", u)
```
Example output with timestamp of type ```ESP32Timestamp::TimestampSinceStart```:
```
00000004120 INF setup: This is an info-example; string-value s: test.
00000004120 ERR setup: This is an error message; uint-value u: 123.
```
Example output with timestamp of type ```ESP32Timestamp::TimestampLocaltime```:
```
23.04.2021 09:39:57 INF setup: This is an info-example; string-value s: test.
23.04.2021 09:39:57 ERR setup: This is an error message; uint-value u: 123.
```
Logging depends on the choosen level of logging; the three levels are:
* Error
* Info
* Debug
With ```DBGCHK``` a conditional logging is possible, i.e. the logging can depend on a certain condition which will be checked during run time of the program. In the following example the message will only be logged if the returned value in ```err``` is not 0:
```C++
uint32_t err = functionA();
DBGCHK(Error, err==0, "functionA returned error-code: %u.", u)
```
Library ESP32Logger is tested with ESP32.

## Outlook
Add other types of output as acutally the serial-output, like e.g. via MQTT.

## Include the library in the project

Include library in the project and activate in general the logging-funcionality (i.e. include code for logging in the project):
```C++
#define ESP32DEBUGGING
#include <ESP32Logger.h>
```
Just comment out the ```#define ESP32DEBUGGING``` definition and no logging-code will be included to the project.

It is not necessary to remove the logging-commands out of the project-code.

## DBGINI: Initialize logging with 

Initialize the logging, usually done in the setup()-function, by one of the four alternatives:
```C++
Serial.begin(115200);
DBGINI(&Serial)                                        // By default equal to "TimestampSinceStart"
DBGINI(&Serial, ESP32Timestamp::TimestampNone)         // w/o timestamp
DBGINI(&Serial, ESP32Timestamp::TimestampSinceStart)   // Timestamp in milli-seconds since start of program
DBGINI(&Serial, ESP32Timestamp::TimestampLocaltime)    // Specific time, has to be set in the beginning
```

## DBGLEV: Set level of logging with 

Change of level of logging (by default logging starts with level ```Info```):
```C++
DBGLEV(Error)   // Only Error-messages will be logged.
DBGLEV(Info)    // Error- and Info-messages will be logged.
DBGLEV(Debug)   // All messages will be logged.
```

## DBGSTA and DBGSTP: Start and end logging during program execution

After initialization with ```DBGINI``` logging has to be startet exsplicatly with
```C++
DBGSTA
```
And can be stopped temporarly with
```C++
DBGSTP
```
And re-started again with ```DBGSTA```.

## DBGLOG: Log a message

```DBGLOG``` logs a message according to the defined level of logging (see also ```DBGLEV```).

The first parameter is the logging-level.

The second parameter is the message to be logged itself.

The following parametes are optional and can be further data to be logged referenced in the logging-message.
The further paramaters can be referenced following the rules for the C++-standard library function [vsnprintf()](https://www.cplusplus.com/reference/cstdio/vsnprintf/).

Example code:
```C++
char s[] = "Test";
uint32_t u = 123;
DBGLOG(Info, "This logs a simple info-message.")
DBGLOG(Debug, "This is an advanced debug-message with text %s and number %u.", s, u)
DBGLOG(Error, "This is an error message; error-code: %u.", u)
```
Example output:
```
00000004120 INF setup: This logs a simple info-message.
00000004120 DBG functionA: This is an advanced debug-message with text Test and number 123.
00000004130 ERR functionB: This is an error message; error-code: 123.
```

## DBGCHK: Conditional logging

With ```DBGCHK``` it is possible to trigger during run time of the program depending on a condition the logging of the message.
The message will only be logged if the condition is *not* fulfilled.

This can be usefull for example in case of
* checking the parameters of a function in the beginning of the function
* checking the return value of a function before ending a function
* checking the return value of a called function.
Example code:
```C++
char* functionZ(uint32_t input) {
    char* cptr = nullptr;
    // functionZ can only handle input > 0
    DBGCHK(Error, input > 0, "functionZ cannot handle input < 1; input: %u.", input)
    // Do something with input and return cptr; ensure that cptr is not null.
    DBGCHK(Error, cptr != nullptr, "Invalid null-pointer to be returned")
    return cptr;
}
```

## DBGCOD: Include temporary debugging-code

With ```DBGCOD``` temporary program code can be included for the only purpose of debugging. Usefull in combination with ```DBGCHK``` to check more complex situations. The code defined with ```DBGCOD``` (as all the other logging-code) will only be included in case ```ESP32DEBUGGING``` is defined.
Example code:
```C++
DBGCOD(unsigned long time1;)
DBGCOD(unsigned long time2;)
DBGCOD(time1 = millis();)
// Do something
DBGCOD(time2 = millis();)
DBGCHK(Info, time2-time1 < 100, "Code execution took long: %lu ms", time2-time1)
```
