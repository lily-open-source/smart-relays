#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>

// Blynk authentication token and Wi-Fi credentials
const char* BLYNK_AUTH_TOKEN = "token";
const char* WIFI_SSID = "YourNetworkName";
const char* WIFI_PASSWORD = "YourPassword";

// DHT sensor settings
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Relay settings
const int RELAY_PINS[] = {D0, D1, D2, D3};

// Blynk virtual pins for relay control
#define V1 1
#define V2 2
#define V3 3
#define V4 4

BlynkTimer timer;

void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  if (isnan(h) || isnan(t)) return;
  delay(1000); // 1 second delay before sending data
  Blynk.virtualWrite(V5, h);
  Blynk.virtualWrite(V6, t);
}

void relaySwitch(int index, int pinValue) {
  digitalWrite(RELAY_PINS[index], pinValue);
}

void setup()
{
  Serial.begin(115200);

  // Wi-Fi connection
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

  // Blynk connection
  Blynk.begin(BLYNK_AUTH_TOKEN, WiFi);

  // DHT sensor initialization
  dht.begin();

  // Relay setup
  for (int i = 0; i < 4; i++) {
    pinMode(RELAY_PINS[i], OUTPUT);
  }

  // Blynk virtual pin handlers for relay control
  Blynk.virtualWrite(V1, LOW);
  Blynk.virtualWrite(V2, LOW);
  Blynk.virtualWrite(V3, LOW);
  Blynk.virtualWrite(V4, LOW);
  Blynk.setHandler(V1, [](const BlynkReq &req) { relaySwitch(0, req.asInt()); });
  Blynk.setHandler(V2, [](const BlynkReq &req) { relaySwitch(1, req.asInt()); });
  Blynk.setHandler(V3, [](const BlynkReq &req) { relaySwitch(2, req.asInt()); });
  Blynk.setHandler(V4, [](const BlynkReq &req) { relaySwitch(3, req.asInt()); });

  // Timer to send DHT sensor data to Blynk app
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}
