
/*
   from here https://42bots.com/tutorials/esp8266-example-wi-fi-access-point-web-server-static-ip-remote-control/
   Title: ESP8266 PIR Demo WiFi Version
   PCB: MLP201154
   Date: 19th March 2022
   Connect to WiFi: ESP_PIR
   Password: password
   Set Browser to: 192.168.4.1
*/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// default IP in AP mode is 192.168.4.1
IPAddress    apIP(192, 168, 4, 1);  // Defining a static IP address: local & gateway

/* This are the WiFi access point settings. Update them to your likin */
const char *ssid = "ESP_PIR";
const char *password = "password";

// Define a web server at port 80 for HTTP
ESP8266WebServer server(80);

const int LED_Pin = 12;           // LED connection
const int PIR_Pin = 5;            // PIR input
int LED_Flash_Enable = 1;         // LED flash with PIR, 1=enabled
int brightness = 0;               // brightness value

// valid page request
void handleRoot() {

  // read the photo sensor
  // turn off LED first
  digitalWrite(LED_Pin, LOW);
  delay(1);
  brightness = analogRead(A0);

  // get the request mode
  LED_Flash_Enable = server.arg("led").toInt();

  // dynamically generate the LED enable mode
  char LED_Mode[80];
  if (LED_Flash_Enable == HIGH) {
    strcpy(LED_Mode, "<p>LED Is <font color='white'>Enabled</font> <a class='button1' href='?led=0'>Disable</a></p>");
  } // end if
  else {
    strcpy(LED_Mode, "<p>LED Is <font color='red'>Disabled</font> <a class='button1' href='?led=1'>Enable</a></p>");
  } // end else

  // test the PIR input
  char PIR_Text[80];
  // test PIR input
  int Test = digitalRead(PIR_Pin);
  if (Test == HIGH) {
    strcpy(PIR_Text, "PIR <b><font color='red'>High</font></b> ");
  } // end if
  else {
    strcpy(PIR_Text, "PIR <font color='white'>Low</font> ");
  } // end else

  char html[1500];
  int sec = millis() / 1000;
  int min = sec / 60;
  int hr = min / 60;


  // Build an HTML page to display on the web-server root address, see https://www.geeksforgeeks.org/snprintf-c-library/
  // note: %s=strings, %d=numbers in decimal, %02d=numbers 2-decimal places.
  snprintf ( html, 1500,

             "<html>\
    <head>\
     <meta http-equiv='refresh' content='1'/>\
     <title>ESP8266 PIR</title>\
     <style>\
        body {\
        background-color: #224466;\
        font-family: Arial, Helvetica, Sans-Serif;\
        font-size: 2em;\
        Color: #FFFFFF; }\
        h1 {\
        font-size: 1.8em;\
        Color: #FFFFFF; }\
        h2 {\
        font-size: 1.2em;\
        Color: #FF8C1A;\
        }\
        .button1 {\
        background-color: #222;\
        border: none;\
        color: #ddd;\
        padding: 0.5vh 1vw 1vh 1vw;\
        text-decoration: none;\
        padding: 0.5vh 1vw 1vh 1vw;\
        border-radius: calc((1vw + 1vh)*0.6) calc((1vw + 1vh)*0.6) calc((1vw + 1vh)*0.6) calc((1vw + 1vh)*0.6);\
        border: solid calc((1vw + 1vh)*0.2) #fff;\
        };\
     </style>\
    </head>\
    <body>\
        <h1>ESP8266 Wi-Fi PIR Demo</h1>\
        <h2>Board: MLP201154</h2>\
        <p>Uptime: %02d:%02d:%02d</p>\
        <p>%s</p>\
        <p>%s</p>\
        <p>Light level: %d</p>\
        <p>This page refreshes every 1 seconds</p>\
    </body>\
  </html>",
             hr, min % 60, sec % 60,
             PIR_Text,
             LED_Mode,
             brightness
           );

  // see https://techtutorialsx.com/2016/10/22/esp8266-webserver-getting-query-parameters/
  // send as 200 not 404 else the html is displayed as text only
  server.send ( 200, "text/html", html );
} // end handle root


void handleNotFound() {

  // 404 message
  String message = "<!DOCTYPE html>";
  message += "<html><head>";
  message += "<title>Error 404</title>";
  message += "<head><body><h1>Error 404 - Page Not Found</h1>";
  message += "<p>The document you are looking for does not exist.</p>";
  message += "</body></html>";
  // send page as html
  server.send ( 200, "text/html", message );

} // end handle 404

void setup() {
  pinMode(LED_Pin, OUTPUT);
  digitalWrite(LED_Pin, LOW);
  pinMode(PIR_Pin, INPUT);

  delay(1000);
  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  //set-up the custom IP address
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00

  /* You can remove the password parameter if you want the AP to be open. */
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);

  // on server request to this
  server.on ( "/", handleRoot );
  server.onNotFound ( handleNotFound );

  server.begin();
  Serial.println("HTTP server started");
} // end setup


void loop() {
  server.handleClient();

  // is the LED enabled, if so then test the PIR
  if (LED_Flash_Enable == HIGH) {

    if (digitalRead(PIR_Pin) == HIGH) {
      digitalWrite(LED_Pin, HIGH);
    } // end if
    else {
      digitalWrite(LED_Pin, LOW);
    } // end else

  } // end if

  else {
    digitalWrite(LED_Pin, LOW);
  } // end else

} // end loop
