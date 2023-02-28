#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
const char* ssid = "your_ssid";
const char* password = "ssid_password";
const int RELAY_PINS[] = {D0, D1, D2, D3};
void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
  }
  Blynk.begin("your_BLYNK_AUTH_TOKEN", ssid, password);
}
void relaySwitch(int index, int pinValue) {
  digitalWrite(RELAY_PINS[index], pinValue);
}
BLYNK_WRITE(V1) {relaySwitch(0, param.asInt());}
BLYNK_WRITE(V2) {relaySwitch(1, param.asInt());}
BLYNK_WRITE(V3) {relaySwitch(2, param.asInt());}
BLYNK_WRITE(V4) {relaySwitch(3, param.asInt());}
void loop() {
  Blynk.run();
}
