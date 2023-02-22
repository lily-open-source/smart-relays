const int RELAY_PINS[] = {16, 5, 4, 0};
const int BUTTON_PINS[] = {2, 14, 12, 13};
int relay_states[] = {0, 0, 0, 0};
int button_states[] = {0, 0, 0, 0};

#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

const char* WLAN_SSID = ".....";
const char* WLAN_PASS = ".....";
const char* AIO_SERVER = "io.adafruit.com";
const int AIO_SERVERPORT = 1883;
const char* AIO_USERNAME = ".....";
const char* AIO_KEY = ".....";

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);
Adafruit_MQTT_Subscribe subscriptions[] = {
  Adafruit_MQTT_Subscribe(&mqtt, String(AIO_USERNAME) + "/feeds/relay1"),
  Adafruit_MQTT_Subscribe(&mqtt, String(AIO_USERNAME) + "/feeds/relay2"),
  Adafruit_MQTT_Subscribe(&mqtt, String(AIO_USERNAME) + "/feeds/relay3"),
  Adafruit_MQTT_Subscribe(&mqtt, String(AIO_USERNAME) + "/feeds/relay4")
};
Adafruit_MQTT_Publish publishers[] = {
  Adafruit_MQTT_Publish(&mqtt, String(AIO_USERNAME) + "/feeds/relay1"),
  Adafruit_MQTT_Publish(&mqtt, String(AIO_USERNAME) + "/feeds/relay2"),
  Adafruit_MQTT_Publish(&mqtt, String(AIO_USERNAME) + "/feeds/relay3"),
  Adafruit_MQTT_Publish(&mqtt, String(AIO_USERNAME) + "/feeds/relay4")
};

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
    pinMode(BUTTON_PINS[i], INPUT_PULLUP);
  }

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }

  for (int i = 0; i < 4; i++) {
    mqtt.subscribe(&subscriptions[i]);
  }
}

void loop() {
  if (!mqtt.connected()) {
    mqtt.connect();
  }

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(20000))) {
    for (int i = 0; i < 4; i++) {
      if (subscription == &subscriptions[i]) {
        relay_states[i] = atoi((const char *)subscriptions[i].lastread);
        digitalWrite(RELAY_PINS[i], relay_states[i]);
      }
    }
  }

  for (int i = 0; i < 4; i++) {
    button_states[i] = digitalRead(BUTTON_PINS[i]);
    if (button_states[i] == LOW) {
      relay_states[i] = !relay_states[i];
      digitalWrite(RELAY_PINS[i], relay_states[i]);
      publishers[i].publish(relay_states[i]);
      delay(50);
    }
  }
}
