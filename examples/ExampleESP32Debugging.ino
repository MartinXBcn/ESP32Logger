/*
 Name:		ExampleESP32Debugging.ino
 Created:	4/21/2021 11:04:52 AM
 Author:	Martin
*/

// Individual definitions (like passwords) for the examples
#include "individual_defs.h"

// Include debugging-code in the program
#define ESP32DEBUGGING
#include <ESP32Logger.h>

// For Wifi-connection
#include <WiFi.h>
const char* ssid = WIFI_SSID;
const char* password = WIFI_PASSWORD;

// To set time
const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 3600;
const int   daylightOffset_sec = 0; // 3600;


uint32_t counter = 0;

void func1(uint32_t& x) {
	// Insert code only for debugging
	DBGCOD(int d = x % 3;)
	DBGCOD(const char* s = "Number";)
	// Check condition
	DBGCHK(Error, d == 0, "%s %u is not a multiple of 3", s, x)
	DBGLOG(Info, "Counter = %u", x)
	Serial.println(x);
	x++;
}

void setupWiFi() {
	DBGLOG(Info, "Wifi connecting to %s", ssid)

	wl_status_t ret;
	int c = 5;
	do {
		if (c > 4) {
			DBGLOG(Info, "WiFi.begin(...)")
			ret = WiFi.begin(ssid, password);
			c = 0;
		}
		DBGLOG(Info, "%i", counter)
		c++;
		delay(500);
		ret = WiFi.status();
	} while (ret != WL_CONNECTED);

	IPAddress ip = WiFi.localIP();
	String s = ip.toString();
	DBGLOG(Info, "WiFi connected, IP address: %s", s.c_str())
} // void setupWiFi()

void setup() {
	Serial.begin(115200);
	delay(50);
	// Initialize debugging with timestamp since start (start = 0) 
	DBGINI(&Serial, ESP32Timestamp::TimestampSinceStart)
	// Start debugging
	DBGSTA
	// Log an info-message
	DBGLOG(Info, "Program ExampleESP32Debugging.ino startet.")
	// Log an error-message
	DBGLOG(Error, "This is an error message.")
	// Log a debug-message, which will not be shown, because actual debug-level is "Info"
	DBGLOG(Debug, "This is an message with logging level 'Debug' which will not be printed.")
	// Change debug-level to "Debug"
	DBGLEV(Debug)
	// The following debug-message will now be shown
	DBGLOG(Debug, "This is an message with logging level 'Debug' which will be printed.")
	// Stop debugging
	DBGSTP
	// While debugging is stopped messages will not be shown
	DBGLOG(Info, "This will not be printed because Debugging is stopped.")
	// Activate debugging again
	DBGSTA
	setupWiFi();
	DBGLOG(Info, "Get actual time.")
	configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
	DBGLOG(Info, "Activate timestamp-mode 'TimestampLocaltime'.")
	// Initialize debugging with timestamp of actual time
	DBGINI(&Serial, ESP32Timestamp::TimestampLocaltime)
	// Start debugging
	DBGSTA
	DBGLOG(Info, "End of setup.")
	Serial.println("End of setup");
} // void setup()

void loop() {
	delay(5000);
	func1(counter);
}
