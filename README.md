# ESP RSSI Measurement

This repository contains two Arduino sketches for measuring the Received Signal Strength Indicator (RSSI) of packets received by an ESP8266 or ESP32 device. The sketches are designed to be uploaded to an ESP8266 or ESP32 device and will display the RSSI of received packets on the device's screen.

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

This project uses ESP-NOW, a protocol developed by Espressif. ESP-NOW enables devices to communicate without the need for Wi-Fi connectivity. This means that the devices do not need to be connected to the same network or any network at all to communicate.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
