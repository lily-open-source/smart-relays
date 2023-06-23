#include <Adafruit_MQTT.h>
#include <Adafruit_MQTT_Client.h>
#include <ESP8266WiFi.h>

// Define your Wi-Fi credentials
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

// Define your Adafruit IO credentials
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "YOUR_ADAFRUIT_IO_USERNAME"
#define AIO_KEY         "YOUR_ADAFRUIT_IO_KEY"

// Define the button pin
const int buttonPin = 2;

// Define the relay pin
const int relayPin = 3;

// Create an instance of the Adafruit IO MQTT client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Create an instance of the Adafruit IO MQTT relay feed
Adafruit_MQTT_Subscribe relayFeed = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay");

char relayState[2] = {'0', '\0'}; // Relay state as a string

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.println(WiFi.localIP());

  // Connect to Adafruit IO
  mqtt.connect();
  relayFeed.setCallback(relayCallback);
  mqtt.subscribe(&relayFeed);

  // Set button pin as input
  pinMode(buttonPin, INPUT_PULLUP);

  // Set relay pin as output
  pinMode(relayPin, OUTPUT);

  // Set the initial state of the relay
  digitalWrite(relayPin, LOW);
}

void loop() {
  // Check for incoming MQTT messages
  mqtt.processPackets(10000);

  // Check the state of the button
  int buttonState = digitalRead(buttonPin);

  // Publish button state to Adafruit IO
  if (buttonState == LOW) {
    publishState(1);
  } else {
    publishState(0);
  }
}

void relayCallback(char* data, uint16_t len) {
  // Create a temporary buffer to hold the MQTT message payload
  char temp[len + 1];
  strncpy(temp, data, len);
  temp[len] = '\0';

  // Update the relay state
  strncpy(relayState, temp, sizeof(relayState));

  if (relayState[0] == '1') {
    digitalWrite(relayPin, HIGH);
    Serial.println("Relay ON");
  } else {
    digitalWrite(relayPin, LOW);
    Serial.println("Relay OFF");
  }
}

void publishState(int state) {
  // Update the relay state as a string
  relayState[0] = state + '0';

  // Publish the message to Adafruit IO
  Adafruit_MQTT_Publish relayPublish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay");
  if (relayPublish.publish(relayState)) {
    Serial.println("Published to Adafruit IO");
  } else {
    Serial.println("Publishing failed");
  }
}
