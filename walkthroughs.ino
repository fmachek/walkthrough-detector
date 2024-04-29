
// NewPing library
#include <NewPing.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
// Defining the pins and maximum measuring distance
#define pinTrigger    D8
#define pinEcho       D2
#define maxDistance 450
// Defining the wifi ssid and password
const char* ssid = ""; // Change to wi-fi name
const char* password = ""; // Change to wi-fi password
// Defining the http request for adding a walkthrough
const char* serverUrl = ""; // Change to the /add request url

// Initizalizing a NewPing object
NewPing sonar(pinTrigger, pinEcho, maxDistance);
int defaultDistance;
int walkthroughAmount = 0;

void setup() {
  Serial.begin(9600);

// First we connect to our wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

// Print wifi info
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

// Measure the default distance during setup
  defaultDistance = sonar.ping_cm();
  Serial.print("Setup distance: ");
  Serial.println(defaultDistance);
}

// Measures an average distance (measures distance 5 times)
int averageDistance(){
  int totalDistance = 0;
  for(int i = 0; i < 5; i++){
    int distance = sonar.ping_cm();
    totalDistance += distance;
    delay(50);
  }
  int average = totalDistance/5;
  return average;
}

// Sends the /add request to the server
void sendAddRequest() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(serverUrl);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    int httpResponseCode = http.POST("data=someData");

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      Serial.print("Response: ");
      Serial.println(response);
    } else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Not connected to Wi-Fi");
  }
}

void loop() {
  // Load the distance from the sensor
  int distance = averageDistance();

  // Delay between readings (50ms)
  delay(50);

  if(distance > 0 && distance <= defaultDistance + 2) {
    if(defaultDistance - distance > 5) {
      Serial.println("Someone is walking through...");
      boolean distanceNormal = false;
      // Start reading until the distance is back to normal
      while(!distanceNormal) {
        delay(50);
        distance = averageDistance();
        
        if(defaultDistance - distance <= 5) {
          distanceNormal = true;
        }
      }

      walkthroughAmount++;
      sendAddRequest();
      Serial.println("Someone walked through!");
      delay(2000);
    }
  } else {
    Serial.println("The object is outside the range.");
    Serial.print("Wrong distance: ");
    Serial.println(distance);
  }
}
