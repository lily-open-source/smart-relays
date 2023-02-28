#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "your_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "your_BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "your_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char ssid[] = "your_ssid";
char pass[] = "ssid_password";

const int RELAY_PIN[] = {16, 5, 4, 0};

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(RELAY_PIN[i], OUTPUT);
  }

  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void relaySwitch(int index, int pinValue) {
  Serial.print("Switch ");
  Serial.print(index + 1);
  Serial.print(" value is: ");
  Serial.println(pinValue);
  digitalWrite(RELAY_PIN[index], pinValue);
}

BLYNK_WRITE(V1) { relaySwitch(0, param.asInt()); }
BLYNK_WRITE(V2) { relaySwitch(1, param.asInt()); }
BLYNK_WRITE(V3) { relaySwitch(2, param.asInt()); }
BLYNK_WRITE(V4) { relaySwitch(3, param.asInt()); }

void loop() {
  Blynk.run();
}
