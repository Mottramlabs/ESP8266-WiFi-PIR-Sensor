## ESP8266 Wi-Fi PIR - MLP201154
![Display-Type-B](Pictures/PIX201154.jpg?raw=true)

#### Description
A PIR motion sensor with onboard ESP8266 Wi-Fi module. This board design uses a 5V supply from a Micro USB connector therefore is not intended for battery or low power use, however this does mean it's always connected to the Wi-Fi and so responses quickly. This makes the board ideal as a room occupancy sensor or if the case design is waterproof it could be used out doors.

>**Range** 
The range of the sensor depends on the Lens used. The small cap style lens (clips onto the sensor) are good for short range detection ideal for proximity sensing. The larger lens (23.5mmx23.5mm) are good for room sensing and and have a range of around 10m depending on ambient temperature. 

The board includes a status LED  mounted under the larger PIR lens along with a light sensor. This makes it easier to design a waterproof case, however the LED should be turned off before sampling the light level else the readings maybe inaccurate.

Programming the board is done via an external USB to serial interface, a 6 pin header on the board provides the connections required, RX TX GPIO0 and Reset.

### Features
- 5V supply (Micro USB or solder pads), with reverse polarity protection
- Status LED Controlled By The ESP8266
- Light Level Sensor (Connected to A0)
- PIR Sensor Used: AM612

### Firmware
Sample code is provided for basic board testing, a simple Wi-Fi demo with access point and an MQTT application.
