#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

#define BLYNK_AUTH_TOKEN            "auth_token"

char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) return;
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void setup()
{
  Serial.begin(115200);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  dht.begin();
  timer.setInterval(1000L, sendSensor);
  
  // Relay setup
  for (int i = 0; i < 4; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
  }
}

const char* ssid = "your_ssid";
const char* password = "ssid_password";
const int RELAY_PINS[] = {D0, D1, D2, D3};

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
