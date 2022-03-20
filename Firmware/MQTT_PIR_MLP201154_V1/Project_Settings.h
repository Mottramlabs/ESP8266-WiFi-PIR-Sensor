/*
    variables and functions used by this project
*/

// Heatbeat items
#define Heartbeat_Range   99                      // event count max value
#define Heatbeat    10000                         // heartbeat timer value, in mS

// Watchdog items
volatile int Watchdog_Timout = 1200;              // time in seconds before watchdog times out, 1200 = 1200 seconds or 20 minutes

// Heatbeat timer
int Heart_Value = 0;                              // timer
long LastMsg = 0;                                 // message sent counter

// Custom default values
String WiFi_SSID = "None";                        // SSID string
String My_MAC = "";                               // MAC address, tobe read from ESP8266
char MAC_array[13] = "000000000000";              // MAC definition
String My_IP = "";                                // IP address

volatile int Report_Request = 0;                  // set to 1 if report required

// ********************************************************************************************************
// ************************ project specific variables ****************************************************
// ********************************************************************************************************

#define LED 12
#define PIR 5
int PIR_Flag = false;
String PIR_Status = "Boot";
int Light_Level = 0;

unsigned long LED_Timer = 0;
#define LED_ON_Time 100

// ********************************************************************************************************
// ************************ watchdog items ****************************************************************
// ********************************************************************************************************


#include <Ticker.h>
Ticker secondTick;
volatile int watchdogCount = 0;

// watchdog timer function, watchdog is clear when the ststus report is requested by a GET request
void ISRwatchdog() {
  watchdogCount++;
  if (watchdogCount == Watchdog_Timout) {
    Serial.println();
    Serial.println("The watchdog bites !!!!!");
    ESP.reset();
  } // end of timeout test
} // end of watchdog count
