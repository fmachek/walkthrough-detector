# Walkthrough Detector
This Arduino-based device detects people walking by, and displays today's amount of walkthroughs on a website.

## Parts
- ESP8266 ESP-12E OTA WeMos D1 CH340 WiFi
- Ultrasonic distance meter 5Pin HY-SRF05
- Two-way 3.3V to 5V logic level convertor
- Breadboard (for the convertor)
- M-M cables
- M-F cables
- 3D printed case

## Server side
The server is realized via Node.js. The ESP board sends a request to the server and a new walkthrough is saved to a JSON file. When the website is loaded, the server loads today's walkthroughs from the file and displays them on the website.
