#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ultrasonic.h>
#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
//#include <ESP8266WiFi.h>

// Wi-Fi credentials
const char* ssid = "wangui";
const char* password = "10998600";

LiquidCrystal_I2C lcd(0x27, 16, 2);
Ultrasonic ultrasonic(10, 11, 20);

// Adafruit IO setup
#define AIO_USERNAME    "liz_22"
#define AIO_KEY         "aio_XuSV60sNoLu0zxmfZbI58YFaJ1Ih"
#define AIO_FEED        "Waste"

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883

//WiFiClient espClient;
Adafruit_MQTT_Client mqtt(&espClient, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// MQTT setup
Adafruit_MQTT_Publish wasteLevel = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/" AIO_FEED);

// Define the minimum and maximum distances for your percentage scale
const int minDistance = 2;
const int maxDistance = 25;

void setup() {
  // Connect to Wi-Fi
  connectToWiFi();

  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {
  int distance = ultrasonic.read();
  int percentage = map(distance, minDistance, maxDistance, 0, 100);
  percentage = constrain(percentage, 0, 100);

  // Publish data to Adafruit IO
  publishToAdafruitIO(percentage);

  // Clear the LCD screen
  lcd.clear();

  // Display the distance as a percentage on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Waste Level is :");
  lcd.setCursor(0, 1);
  lcd.print(percentage);
  lcd.print("%");

  // Print the distance and percentage to the serial monitor
  Serial.print("Waste Level is : ");
  Serial.print(distance);
  Serial.print(" cm (");
  Serial.print(percentage);
  Serial.println("%)");

  delay(3000);
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
}

void publishToAdafruitIO(int percentage) {
  // Publish waste level data to Adafruit IO
  if (!mqtt.connected()) {
    connectToAdafruitIO();
  }

  wasteLevel.publish(percentage);
}

void connectToAdafruitIO() {
  // Connect to Adafruit IO
  Serial.print("Connecting to Adafruit IO...");
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    delay(5000);
  }
  Serial.println("Connected to Adafruit IO!");
}
