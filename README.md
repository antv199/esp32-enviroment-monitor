# 🌡️ IoT Environmental Monitor (ESP32 & Thinger.io)

An IoT system for real-time environmental monitoring based on the **ESP32** microcontroller. The device collects temperature and humidity data, processes it locally (Edge Computing), and transmits it to the **Thinger.io** cloud platform for visualization and remote control.

## 🚀 Features

* **Real-time Monitoring:** Reads Temperature & Humidity using the **DHT11** sensor.
* **Cloud Integration:** Streams data via Wi-Fi to **Thinger.io** using the IOTMP protocol.
* **Edge Computing:** Calculates the **5-minute rolling average** of the temperature locally on the ESP32 before transmission, reducing noise.
* **Local Feedback:** Displays live data, connection status, and IP address on an **I2C LCD (16x2)**.
* **Remote Control:** Supports bi-directional communication to toggle a status LED remotely via the cloud dashboard.

## 🛠️ Hardware Stack

* **Microcontroller:** ESP32 Development Board
* **Sensors:** DHT11 (Temperature & Humidity)
* **Display:** LCD 1602 with I2C Module
* **Actuators:** LED (for remote control status)

## 🔌 Pinout Configuration

| Component | ESP32 Pin |
| :--- | :--- |
| **DHT11 Data** | GPIO 2 |
| **LED** | GPIO 3 |
| **LCD SDA** | GPIO 8 |
| **LCD SCL** | GPIO 9 |

## 💻 Software & Libraries

This project is developed using the **Arduino IDE** / **PlatformIO**.

**Required Libraries:**
* `ThingerESP32` (by Thinger.io)
* `DHT sensor library` (by Adafruit)
* `LiquidCrystal_I2C`

## ⚙️ Setup & Installation

1.  Clone the repository.
2.  Open the project in Arduino IDE.
3.  Create a file named `arduino_secrets.h` in the same folder and add your credentials:
    ```cpp
    #define USERNAME "YOUR_THINGER_USERNAME"
    #define DEVICE_ID "YOUR_DEVICE_ID"
    #define DEVICE_CREDENTIAL "YOUR_DEVICE_KEY"
    #define SSID "YOUR_WIFI_SSID"
    #define SSID_PASSWORD "YOUR_WIFI_PASSWORD"
    ```
4.  Upload the code to your ESP32.

## 👥 Team & Roles

* **Antonios Vatousis** - *Software Engineering & Cloud Implementation*
    * Firmware development (C++), Sensor logic, Thinger.io configuration.
* **[Nikolaos Giatras](https://www.linkedin.com/in/nikos-giatras-893344367/)** - *Hardware Engineering*
    * Circuit design, Component assembly, Hardware testing.

---
*Developed as part of the "Internet of Things" course at the University of Ioannina (2024-2025).*
