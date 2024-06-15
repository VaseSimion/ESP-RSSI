# ESP RSSI Measurement

This repository contains two Arduino sketches for measuring the Received Signal Strength Indicator (RSSI) of packets received by an ESP8266 or ESP32 device. The sketches are designed to be uploaded to an ESP8266 or ESP32 device and will display the RSSI of received packets on the device's screen.
I searched the internet a while to be able to measure the RSSI for ESP-now between 2 ESP8266 devices and got bits and pieces from here and there. In this repo you have it all in one place.
I used a small oled screen to show the RSSI to be able to test outside, but you can ignore that and just look on the serial monitor if you do not have one.

## Files

- `ESP8266RSSIMeasurement.ino`: This sketch is for ESP8266 devices. It uses the device's promiscuous mode to capture packets and calculate their RSSI.

- `ESP32RSSIMeasurement.ino`: This sketch is for ESP32 devices. It also uses the device's promiscuous mode to capture packets and calculate their RSSI.

## Requirements

- ESP8266 or ESP32 device
- Another ESP device to send messages
- Arduino IDE for uploading the sketches to the devices

## Usage

1. Upload the appropriate sketch to your ESP8266 or ESP32 device using the Arduino IDE.
2. Set up another ESP device to send messages.
3. Power on the ESP device with the uploaded sketch. It will start capturing packets and displaying their RSSI on the screen.

## Note
For measuring RSSI you do not need to send messages from another ESP to the one that measures RSSI. In promiscuous mode the RSSI with the code in this repo will pick from the air all wifi messages (even if they are not directed to it) and it will filter them to look only at the ones sent by an ESP device and will show the signal strength of those. 
Each device has an OUI number in the wifi message and ESP-now is like wifi which is an identifier of the manufacturer.

This project uses ESP-NOW, a protocol developed by Espressif. ESP-NOW enables devices to communicate without the need for Wi-Fi connectivity. This means that the devices do not need to be connected to the same network or any network at all to communicate.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
