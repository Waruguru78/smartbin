#include <WiFi.h>
#include <Ultrasonic.h>
#include <ThingSpeak.h>

// WiFi credentials
const char* ssid = "TECNO CAMON 18";
const char* password = "Waruguru01";

// ThingSpeak credentials
const char* server = "api.thingspeak.com";
const char* api_key = "R5VVFOJXF0U3EY9R";
const int channel_id = 2361812; // Replace with your ThingSpeak channel ID

// Define the ultrasonic sensor pins
const int triggerPin = 5; // Adjust the pin based on your wiring
const int echoPin = 2;    // Adjust the pin based on your wiring

// Constants for distance calculation
const int maxDistance = 20; // Maximum distance in centimeters

// Create an Ultrasonic object
Ultrasonic ultrasonic(triggerPin, echoPin);

WiFiClient client;

void setup() {
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Initialize ThingSpeak
  ThingSpeak.begin(client);
}

void loop() {
  // Measure waste levels using ultrasonic sensor
  float distance = ultrasonic.read();
  
  // Ensure the distance is within the valid range (0 to maxDistance)
  distance = constrain(distance, 0, maxDistance);

  // Calculate waste level percentage
  int wasteLevelPercentage = map(distance, 0, maxDistance, 100, 0);

  // Update ThingSpeak channel with waste level data
  ThingSpeak.writeField(channel_id, 1, wasteLevelPercentage, api_key);

  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.print(" cm, Waste Level: ");
  Serial.print(wasteLevelPercentage);
  Serial.println("%");

  // Add delay before next update
  delay(5000);  // Update every 5 seconds
}
