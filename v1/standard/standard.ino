/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in information from Blynk Device Info here */
#define BLYNK_TEMPLATE_ID "your_BLYNK_TEMPLATE_ID"
#define BLYNK_TEMPLATE_NAME "your_BLYNK_TEMPLATE_NAME"
#define BLYNK_AUTH_TOKEN "your_BLYNK_AUTH_TOKEN"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "your_ssid";
char pass[] = "ssid_password";

const int Relay1 = 16;  // D0 on NodeMCU
const int Relay2 = 5;   // D1 on NodeMCU
const int Relay3 = 4;   // D2 on NodeMCU
const int Relay4 = 0;   // D3 on NodeMCU

void setup()
{
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);
  // Debug console
  Serial.begin(9600);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

BLYNK_WRITE(V1)
{
  int pinValue = param.asInt();
  Serial.print("Switch 1 value is: ");
  Serial.println(pinValue);
  digitalWrite(Relay1, pinValue);
}

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt();
  Serial.print("Switch 2 value is: ");
  Serial.println(pinValue);
  digitalWrite(Relay2, pinValue);
}

BLYNK_WRITE(V3)
{
  int pinValue = param.asInt();
  Serial.print("Switch 3 value is: ");
  Serial.println(pinValue);
  digitalWrite(Relay3, pinValue);
}

BLYNK_WRITE(V4)
{
  int pinValue = param.asInt();
  Serial.print("Switch 4 value is: ");
  Serial.println(pinValue);

  digitalWrite(Relay4, pinValue);
}

void loop()
{
  Blynk.run();
}
