# ESP8266 Relay Control with Push Button and MQTT

This code is for controlling relays with push button and MQTT on an ESP8266 board.

## Hardware Requirements
- ESP8266 board
- Relays (4)
- Push buttons (4)
- Wires
- Breadboard (optional)

## Software Requirements
- Arduino IDE
- ESP8266 Board Package
- Adafruit MQTT library

## Setup
1. Install the ESP8266 board package and the necessary libraries (ESP8266WiFi, Adafruit_MQTT, and Adafruit_MQTT_Client) in the Arduino IDE.
2. Replace the placeholder values in the code with your own WiFi SSID and password, MQTT server, username, and key.
3. Assign the correct digital pins for the relays and push buttons in the code.
4. Upload the code to your ESP8266 board.
5. Connect the board to a power source and make sure it is connected to the same network as your MQTT server.

## Usage
- The code will automatically subscribe to the MQTT feeds for each relay and update the state of the relays based on the values received from the MQTT feeds.
- When you press the push button for a specific relay, it will toggle the state of the relay and update the value on the MQTT feed.
- The relays can also be controlled remotely by publishing new values to the MQTT feeds, the ESP8266 board will read the new values and update the state of the relays accordingly.

## Note
This code uses the Adafruit MQTT library, you can use other libraries as well.
