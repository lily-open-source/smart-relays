//define pins
#define Relay1 D1
#define Relay2 D2
#define Relay3 D3
#define Relay4 D4

//library
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

//wifi config
#define WLAN_SSID "....."       // Your SSID
#define WLAN_PASS "....."    // Your password

//aio config
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883      // use 8883 for SSL
#define AIO_USERNAME "....."
#define AIO_KEY "....."

//wifi client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//mqtt subscribe
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/relay1");
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/relay2");
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/relay3");
Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/relay4");

void setup() {
  //set pins as output
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  //connect to wifi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  //subscribe to mqtt feeds
  mqtt.subscribe(&Light1);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&Light3);
  mqtt.subscribe(&Light4);
}

void loop() {
  //connect to mqtt
  if (!mqtt.connected()) {
    mqtt.connect();
  }

  //check for new data
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    if (subscription == &Light1) {
      int Light1_State = atoi((const char *)Light1.lastread);
      digitalWrite(Relay1, Light1_State);
    }
    if (subscription == &Light2) {
      int Light2_State = atoi((const char *)Light2.lastread);
      digitalWrite(Relay2, Light2_State);
    }
    if (subscription == &Light3) {
      int Light3_State = atoi((const char *)Light3.lastread);
      digitalWrite(Relay3, Light3_State);
    }
    if (subscription == &Light4) {
      int Light4_State = atoi((const char *)Light4.lastread);
      digitalWrite(Relay4, Light4_State);
    }
  }
}
