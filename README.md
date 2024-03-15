# SMART_STICK_FOR_VISION_CHALLENEGED

# Smart Stick Monitoring System with GPS Monitoring on Blynk

This project implements a Smart Stick Monitoring System using an ESP32 microcontroller and a GPS module. It sends GPS data to the Blynk app for real-time tracking and monitoring.

## Hardware Requirements

- ESP32 WROOM-DA-MODULE
- GPS Module (e.g., NEO-6M)
- USB Cable for ESP32 Programming
- Smartphone with Blynk App Installed

## Software Requirements

- Arduino IDE
- Blynk App (available for Android and iOS)

## Setup Instructions

1. Clone or download the repository to your local machine.
2. Open the Arduino IDE and install the necessary libraries:
   - WiFiClient
   - BlynkSimpleEsp32
   - TinyGPS++
3. Modify the following in the code (`smart_stick_monitor.ino`):
   - WiFi SSID and Password (`ssid` and `pass` variables)
   - Blynk Template ID, Name, and Authentication Token (`BLYNK_TEMPLATE_ID`, `BLYNK_TEMPLATE_NAME`, `BLYNK_AUTH_TOKEN` variables)
   - GPS Serial Pins (`RXD2` and `TXD2` pins)
4. Connect the ESP32 board to your computer using the USB cable.
5. Select the appropriate board and port in the Arduino IDE.
6. Upload the code (`smart_stick_monitor.ino`) to the ESP32 board.
7. Open the Blynk app on your smartphone and load the corresponding project using the Template ID.

## Usage

1. Power on the ESP32 board and GPS module.
2. Launch the Blynk app on your smartphone.
3. Monitor the GPS data (latitude, longitude, speed) on the Blynk app in real-time.
4. Use the provided virtual pins (`V0`, `V1`, `V2`, `V3`) in the Blynk app for displaying GPS data and map links.

## Contributing

Contributions to improve this project are welcome. You can fork the repository, make your changes, and create a pull request.

## License

This project is licensed under the [MIT License](LICENSE).
