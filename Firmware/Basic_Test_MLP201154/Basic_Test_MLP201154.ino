/*
    Title:        MLP20154 Test
    Date:         11th February 2022
    Version:      1
    Description:  Sample code
    Device:       ESP8266
*/

#include <ESP8266WiFi.h>              // needed for EPS8266
#include <WiFiClient.h>               // WiFi client

#define LED 12
#define PIR 5
int PIR_Flag = false;


void setup() {

  Serial.begin(115200); Serial.println(""); Serial.println(""); Serial.println("Up and Clackin!");
  Serial.println(__FILE__);

  // I/O
  pinMode(LED, OUTPUT);
  pinMode(PIR, INPUT);

  // default mode
  digitalWrite(LED, HIGH);
  delay(100);
  digitalWrite(LED, LOW);

} // end of setup


void loop() {

  // read the photo sensor
  // turn off LED first
  digitalWrite(LED, LOW);
  delay(1);
  int sensorValue = analogRead(A0);
  // print the results to the Serial Monitor:
  //  Serial.print("A/D = "); Serial.println(sensorValue);
  Serial.println(sensorValue);

  // test PIR input
  int Test = digitalRead(PIR);
  if (Test == HIGH) {

    if (PIR_Flag == false) {
      PIR_Flag = true;
      digitalWrite(LED, HIGH);
      delay(50);
      digitalWrite(LED, LOW);
    } // end if

  } // end if
  else {
    digitalWrite(LED, LOW);
    PIR_Flag = false;
  } // end else

  //  digitalWrite(LED, HIGH);
  delay(500);

} // end of loop
