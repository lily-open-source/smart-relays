// Relay pins
const int Relay1 = 16;  // D0 on NodeMCU
const int Relay2 = 5;   // D1 on NodeMCU
const int Relay3 = 4;   // D2 on NodeMCU
const int Relay4 = 0;   // D3 on NodeMCU

// Button pins
const int Relay1_Button = 2;   // D4 on NodeMCU
const int Relay2_Button = 14;  // D5 on NodeMCU
const int Relay3_Button = 12;  // D6 on NodeMCU
const int Relay4_Button = 13;  // D7 on NodeMCU

// Initialize the relay states as false
bool Relay1_State = false;
bool Relay2_State = false;
bool Relay3_State = false;
bool Relay4_State = false;

// Initialize the button states as false
bool Relay1_Button_State = false;
bool Relay2_Button_State = false;
bool Relay3_Button_State = false;
bool Relay4_Button_State = false;

// Initialize the reboot states as false
bool reboot_flag = false;

// save the counter value
int counter = 0;

// include libraries
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

// WiFi config
#define WLAN_SSID "your-ap"       // Your SSID
#define WLAN_PASS "ap_pass"    // Your password

// Adafruit IO config
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883      // use 8883 for SSL
#define AIO_USERNAME "aio_username"
#define AIO_KEY "aio_key"

// WiFi client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// MQTT subscribe
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay1");
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay2");
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay3");
Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/relay4");

// MQTT publish
Adafruit_MQTT_Publish Light1_Publish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay1");
Adafruit_MQTT_Publish Light2_Publish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay2");
Adafruit_MQTT_Publish Light3_Publish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay3");
Adafruit_MQTT_Publish Light4_Publish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/relay4");

void setup() {
  // Set relay pins as output
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  // Set button pin as input pullup
  pinMode(Relay1_Button, INPUT_PULLUP);
  pinMode(Relay2_Button, INPUT_PULLUP);
  pinMode(Relay3_Button, INPUT_PULLUP);
  pinMode(Relay4_Button, INPUT_PULLUP);

  // Initialize serial communication
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  // Start the web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", "text/html");
  });

  // Serve static files from SPIFFS
  server.serveStatic("/", SPIFFS, "/");

  // Start the server
  server.begin();

  // Initialize MQTT subscriptions
  mqtt.subscribe(&Light1);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&Light3);
  mqtt.subscribe(&Light4);
}

void loop() {
  // Reconnect WiFi if connection is lost
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi connection lost. Reconnecting...");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi reconnected");
  }

  // Maintain MQTT connection
  mqtt.loop();

  // Check if the MQTT feeds have received new data
  if (Light1_Publish.publishing()) {
    if (Relay1_State != Light1_Publish.read().toInt()) {
      Relay1_State = !Relay1_State;
      digitalWrite(Relay1, Relay1_State);
      Serial.print("Relay 1: ");
      Serial.println(Relay1_State);
    }
  }

  if (Light2_Publish.publishing()) {
    if (Relay2_State != Light2_Publish.read().toInt()) {
      Relay2_State = !Relay2_State;
      digitalWrite(Relay2, Relay2_State);
      Serial.print("Relay 2: ");
      Serial.println(Relay2_State);
    }
  }

  if (Light3_Publish.publishing()) {
    if (Relay3_State != Light3_Publish.read().toInt()) {
      Relay3_State = !Relay3_State;
      digitalWrite(Relay3, Relay3_State);
      Serial.print("Relay 3: ");
      Serial.println(Relay3_State);
    }
  }

  if (Light4_Publish.publishing()) {
    if (Relay4_State != Light4_Publish.read().toInt()) {
      Relay4_State = !Relay4_State;
      digitalWrite(Relay4, Relay4_State);
      Serial.print("Relay 4: ");
      Serial.println(Relay4_State);
    }
  }

  // Check if the button states have changed
  if (digitalRead(Relay1_Button) != Relay1_Button_State) {
    Relay1_Button_State = !Relay1_Button_State;
    if (Relay1_Button_State) {
      Relay1_State = !Relay1_State;
      digitalWrite(Relay1, Relay1_State);
      Serial.print("Relay 1: ");
      Serial.println(Relay1_State);
    }
    delay(50); // debounce delay
  }

  if (digitalRead(Relay2_Button) != Relay2_Button_State) {
    Relay2_Button_State = !Relay2_Button_State;
    if (Relay2_Button_State) {
      Relay2_State = !Relay2_State;
      digitalWrite(Relay2, Relay2_State);
      Serial.print("Relay 2: ");
      Serial.println(Relay2_State);
    }
    delay(50); // debounce delay
  }

  if (digitalRead(Relay3_Button) != Relay3_Button_State) {
    Relay3_Button_State = !Relay3_Button_State;
    if (Relay3_Button_State) {
      Relay3_State = !Relay3_State;
      digitalWrite(Relay3, Relay3_State);
      Serial.print("Relay 3: ");
      Serial.println(Relay3_State);
    }
    delay(50); // debounce delay
  }

  if (digitalRead(Relay4_Button) != Relay4_Button_State) {
    Relay4_Button_State = !Relay4_Button_State;
    if (Relay4_Button_State) {
      Relay4_State = !Relay4_State;
      digitalWrite(Relay4, Relay4_State);
      Serial.print("Relay 4: ");
      Serial.println(Relay4_State);
    }
    delay(50); // debounce delay
  }
}
