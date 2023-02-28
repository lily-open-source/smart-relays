# ESP8266 Relay Control with Blynk
This is a simple Arduino sketch that allows you to control 4 relays with an ESP8266 board using the Blynk app.

## Hardware Requirements
- NodeMCU or other ESP8266-based board
- 4-channel relay module
- Wires

## Installation
1. Download and install the Arduino IDE.
2. Connect the NodeMCU board to your computer.
3. Open the Arduino IDE and navigate to File > Preferences.
4. In the Additional Boards Manager URLs field, add the following URL:
`http://arduino.esp8266.com/stable/package_esp8266com_index.json`
5. Navigate to **Tools > Board > Boards Manager** and search for "esp8266". Install the board package.
6. In the Arduino IDE, navigate to **Sketch > Include Library > Manage Libraries** and search for "Blynk". Install the Blynk library.
7. Copy and paste the code from this repository into a new sketch in the Arduino IDE.
8. Modify the `ssid`, `pass`, `BLYNK_TEMPLATE_ID`, `BLYNK_TEMPLATE_NAME`, and `BLYNK_AUTH_TOKEN` values to match your WiFi and Blynk credentials.
9. Connect the relay module to the NodeMCU board, making sure to connect the relay signal pins to pins D0, D1, D2, and D3 on the NodeMCU board.
10. Upload the sketch to the NodeMCU board.

## Usage
1. Open the Blynk app and create a new project.
2. Select the ESP8266 board and choose your WiFi network.
3. Add four buttons to the app and assign them to virtual pins V1-V4.
4. Run the app and test the buttons to control the relays.

## License
This code is licensed under the MIT License. See the LICENSE file for details.
