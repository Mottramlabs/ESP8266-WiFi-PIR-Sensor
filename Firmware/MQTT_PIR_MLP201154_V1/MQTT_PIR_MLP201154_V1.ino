/*
    Title:        MQTT PIR Sensor - MLP201154
    Date:         12th February 2022
    Version:      1
    Description:  Sample code
    Device:       ESP8266
    Notes:        Sends events via MQTT, on motion and also when the PIR sensor is cleared. Adds raw A/D value from light sensor
    WiFi:         See "WiFi_Settings.h" for wifi AP and password
    MQTT:         See "MQTT_Settings.h" for MQTT broker and topic
*/

/* ********************************** Compiler settings, un-comment to use ************************** */
//#define Fixed_IP                      // un-comment to use a fixed IP address to speed up development
//#ifdef Watchdog_ON                    // watchdog items, comment out if not used
//#define Print_Report_Level_1          // un-comment for option
#define Print_Report_Level_2          // un-comment for option, report received MQTT message
#define Print_Report_Level_3          // un-comment for option
/* ************************************************************************************************** */

#include <ESP8266WiFi.h>              // needed for EPS8266
#include <WiFiClient.h>               // WiFi client

// custom settings files
#include "Wifi_Settings.h"            // custom Wifi settings
#include "MQTT_Settings.h"            // MQTT broker and topic
#include "Project_Settings.h"         // board specific details.

// incude WiFi and MQTT functions
WiFiClient espClient;                 // for ESP8266 boards
#include <PubSubClient.h>             // http://pubsubclient.knolleary.net/api.html
PubSubClient client(espClient);       // ESP pubsub client
#include "WiFi_Functions.h"           // read wifi data
#include "MQTT_Functions.h"           // MQTT Functions


void setup() {

  Serial.begin(115200); Serial.println(""); Serial.println(""); Serial.println("Up and Clackin!");
  Serial.println(__FILE__);

  // I/O
  pinMode(LED, OUTPUT);
  pinMode(PIR, INPUT);

  // connect to WiFi access point
  Get_Wifi();

  // connect to the MQTT broker
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  // reset heartbeat timer
  LastMsg = millis();

} // end of setup


void loop() {

  if (!client.connected()) {
    reconnect();
  } // end if
  client.loop();

  // LED off
  if (millis() - LastMsg > LED_ON_Time) {
    digitalWrite(LED, LOW);
  } // end if

  // test PIR
  if (digitalRead(PIR) == HIGH) {

    if (PIR_Flag == false) {
      PIR_Flag = true;
      PIR_Status = "Event";
      Report_Request = 1;

      // LED on timer      
      LED_Timer = LastMsg = millis();
      digitalWrite(LED, HIGH);
    } // end if

    else {
      PIR_Status = "High";
    } // end else

  } // end if

  // PIR is low
  else {

    if (PIR_Flag == true) {
      PIR_Flag = false;
      PIR_Status = "Low";
      Report_Request = 1;
    } // end if

  } // end else


  // headbeat or report requested
  if (millis() - LastMsg > Heatbeat || Report_Request == 1) {

    LastMsg = millis();
    Report_Request = 0;

    // update event progress counter
    ++Heart_Value;
    if (Heart_Value > Heartbeat_Range) {
      Heart_Value = 1;
    } // end if

    // heartbeat timed out or report message requested

    // sample light level. don't do if new event as the LED maybe cause an error
    if (PIR_Status != "Event") {
      Light_Level = analogRead(A0);
    } // end if


    // get a report make and make as an array
    String Report = Status_Report();
    char Report_array[(Report.length() + 1)];
    Report.toCharArray(Report_array, (Report.length() + 1));

#ifdef Print_Report_Level_2
    // display a report when publishing
    Serial.print("Published To topic: "); Serial.print(InStatus); Serial.print("  -  Report Sent: "); Serial.println(Report_array);
#endif
    // send a status report
    client.publish(InStatus, Report_array);

  } // end of heartbeat timer

} // end of loop
