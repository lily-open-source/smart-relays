<!DOCTYPE html>
<html>
<body>
  <h1>MQTT Relay Control</h1>
  <p>This code demonstrates how to control relays using MQTT protocol and ESP8266.</p>
  <h2>Hardware</h2>
  <ul>
    <li>ESP8266</li>
    <li>Relay Module</li>
  </ul>
  <h2>Libraries</h2>
  <ul>
    <li>ESP8266WiFi</li>
    <li>Adafruit_MQTT</li>
    <li>Adafruit_MQTT_Client</li>
  </ul>
  <h2>Wifi Configuration</h2>
  <ul>
    <li>Change the value of WLAN_SSID to your wifi name.</li>
    <li>Change the value of WLAN_PASS to your wifi password.</li>
  </ul>
  <h2>MQTT Configuration</h2>
  <ul>
    <li>Change the value of AIO_SERVER to your MQTT broker server.</li>
    <li>Change the value of AIO_SERVERPORT to the port number of your MQTT broker.</li>
    <li>Change the value of AIO_USERNAME to your MQTT username.</li>
    <li>Change the value of AIO_KEY to your MQTT KEY.</li>
   </ul>
   <h2>How it works</h2>
  <p>The code starts by connecting to the WiFi network specified in the wifi configuration. Once connected, it subscribes to 4 different MQTT feeds, one for each relay. In the loop, it will check for new data from the subscribed feeds and if there is new data, it will update the state of the corresponding relay accordingly. </p>
  <h2>Notes</h2>
  <p> Make sure to update the pin configuration according to your setup.</p>
</body>
</html>
