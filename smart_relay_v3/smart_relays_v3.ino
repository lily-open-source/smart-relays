#define Relay1 D0
#define Relay2 D1
#define Relay3 D2
#define Relay4 D3

#define Relay1_Button D7
#define Relay2_Button D6
#define Relay3_Button D5
#define Relay4_Button D4

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

// include librarys
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <ESPAsyncWebServer.h>
AsyncWebServer server(80);

//wifi config
#define WLAN_SSID "ap_name"    // Your SSID
#define WLAN_PASS "ap_pass"    // Your password

//aio config
#define AIO_SERVER "io.adafruit.com"
#define AIO_SERVERPORT 1883      // use 8883 for SSL
#define AIO_USERNAME "aio_username"
#define AIO_KEY "aio_key"

//wifi client
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

//mqtt subscribe
Adafruit_MQTT_Subscribe Light1 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/relay1");
Adafruit_MQTT_Subscribe Light2 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/relay2");
Adafruit_MQTT_Subscribe Light3 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/relay3");
Adafruit_MQTT_Subscribe Light4 = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME"/feeds/relay4");

//mqtt publish
Adafruit_MQTT_Publish Light1_Publish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME"/feeds/relay1");
Adafruit_MQTT_Publish Light2_Publish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME"/feeds/relay2");
Adafruit_MQTT_Publish Light3_Publish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME"/feeds/relay3");
Adafruit_MQTT_Publish Light4_Publish = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME"/feeds/relay4");

void setup() {
  //set relay pins as output
  pinMode(Relay1, OUTPUT);
  pinMode(Relay2, OUTPUT);
  pinMode(Relay3, OUTPUT);
  pinMode(Relay4, OUTPUT);

  //set button pin as input pullup
  pinMode(Relay1_Button, INPUT_PULLUP);
  pinMode(Relay2_Button, INPUT_PULLUP);
  pinMode(Relay3_Button, INPUT_PULLUP);
  pinMode(Relay4_Button, INPUT_PULLUP);

  //connect to wifi
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    counter++;
    if (counter == 10) {
      WiFi.softAP("smart_relays", "drowssap");
      break;
    }
  }

  //subscribe to mqtt feeds
  mqtt.subscribe(&Light1);
  mqtt.subscribe(&Light2);
  mqtt.subscribe(&Light3);
  mqtt.subscribe(&Light4);

  //start the web server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request) {
    String html = "<html><body>";
    html += "<form action='/Relay1' method='POST'>";
    html += "<input type='submit' value='Toggle Relay 1' style='font-size: 18px; padding: 30px;' />";
    html += "<br><br>Relay 1: ";
    if (Relay1_State) {
      html += "<span style='color: green; font-size: 12px;'>ON</span>";
    } else {
      html += "<span style='color: red; font-size: 12px;'>OFF</span>";
    }
    html += "</form><br>";

    html += "<form action='/Relay2' method='POST'>";
    html += "<input type='submit' value='Toggle Relay 2' style='font-size: 18px; padding: 30px;' />";
    html += "<br><br>Relay 2: ";
    if (Relay2_State) {
      html += "<span style='color: green; font-size: 12px;'>ON</span>";
    } else {
      html += "<span style='color: red; font-size: 12px;'>OFF</span>";
    }
    html += "</form><br>";

    html += "<form action='/Relay3' method='POST'>";
    html += "<input type='submit' value='Toggle Relay 3' style='font-size: 18px; padding: 30px;' />";
    html += "<br><br>Relay 3: ";
    if (Relay3_State) {
      html += "<span style='color: green; font-size: 12px;'>ON</span>";
    } else {
      html += "<span style='color: red; font-size: 12px;'>OFF</span>";
    }
    html += "</form><br>";

    html += "<form action='/Relay4' method='POST'>";
    html += "<input type='submit' value='Toggle Relay 4' style='font-size: 18px; padding: 30px;' />";
    html += "<br><br>Relay 4: ";
    if (Relay4_State) {
      html += "<span style='color: green; font-size: 12px;'>ON</span>";
    } else {
      html += "<span style='color: red; font-size: 12px;'>OFF</span>";
    }
    html += "</form><br>";

    html += "<form action='/ToggleAll' method='POST'>";
    html += "<input type='submit' value='Toggle All' style='font-size: 18px; padding: 30px;' />";
    html += "</form><br>";

    // Add a reboot toggle in the web UI
    html += "<form action='/reboot' method='POST'>";
    html += "<input type='submit' value='Reboot' style='font-size: 18px; padding: 30px;' />";
    html += "</form></body></html>";

    html += "</body></html>";
    request->send(200, "text/html", html);
  });

  server.on("/Relay1", HTTP_POST, [](AsyncWebServerRequest * request) {
    Relay1_State = !Relay1_State;
    digitalWrite(Relay1, Relay1_State);
    Light1_Publish.publish(Relay1_State);
    request->redirect("/");
  });

  server.on("/Relay2", HTTP_POST, [](AsyncWebServerRequest * request) {
    Relay2_State = !Relay2_State;
    digitalWrite(Relay2, Relay2_State);
    Light2_Publish.publish(Relay2_State);
    request->redirect("/");
  });

  server.on("/Relay3", HTTP_POST, [](AsyncWebServerRequest * request) {
    Relay3_State = !Relay3_State;
    digitalWrite(Relay3, Relay3_State);
    Light3_Publish.publish(Relay3_State);
    request->redirect("/");
  });

  server.on("/Relay4", HTTP_POST, [](AsyncWebServerRequest * request) {
    Relay4_State = !Relay4_State;
    digitalWrite(Relay4, Relay4_State);
    Light4_Publish.publish(Relay4_State);
    request->redirect("/");
  });

  server.on("/ToggleAll", HTTP_POST, [](AsyncWebServerRequest * request) {
    Relay1_State = !Relay1_State;
    digitalWrite(Relay1, Relay1_State);
    Light1_Publish.publish(Relay1_State);

    Relay2_State = !Relay2_State;
    digitalWrite(Relay2, Relay2_State);
    Light2_Publish.publish(Relay2_State);

    Relay3_State = !Relay3_State;
    digitalWrite(Relay3, Relay3_State);
    Light3_Publish.publish(Relay3_State);

    Relay4_State = !Relay4_State;
    digitalWrite(Relay4, Relay4_State);
    Light4_Publish.publish(Relay4_State);

    request->redirect("/");
  });

  server.on("/reboot", HTTP_POST, [](AsyncWebServerRequest * request) {
    ESP.restart();
  });

  server.begin();
}

void loop() {
  //connect to mqtt
  if (!mqtt.connected()) {
    mqtt.connect();
  }

  //check for new data
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(10000))) {
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

  //Check the state of the push button
  Relay1_Button_State = digitalRead(Relay1_Button);
  if (Relay1_Button_State == LOW) {
    Relay1_State = !Relay1_State;
    digitalWrite(Relay1, Relay1_State);
    Light1_Publish.publish(Relay1_State);
    delay(50);
  }

  Relay2_Button_State = digitalRead(Relay2_Button);
  if (Relay2_Button_State == LOW) {
    Relay2_State = !Relay2_State;
    digitalWrite(Relay2, Relay2_State);
    Light2_Publish.publish(Relay2_State);
    delay(50);
  }

  Relay3_Button_State = digitalRead(Relay3_Button);
  if (Relay3_Button_State == LOW) {
    Relay3_State = !Relay3_State;
    digitalWrite(Relay3, Relay3_State);
    Light3_Publish.publish(Relay3_State);
    delay(50);
  }

  Relay4_Button_State = digitalRead(Relay4_Button);
  if (Relay4_Button_State == LOW) {
    Relay4_State = !Relay4_State;
    digitalWrite(Relay4, Relay4_State);
    Light4_Publish.publish(Relay4_State);
    delay(50);
  }
}