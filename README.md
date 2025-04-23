# ESP32 Home Alarm System

A simple door‐entry alarm built on an ESP32 that:

- Monitors a PIR motion sensor  
- Sounds a buzzer when motion is detected  
- Sends a Telegram notification to your phone  

---

## Features

- **Arm/Disarm** via a pushbutton  
- **Debounced** PIR rising‐edge detection  
- **5 s buzzer alert** + **Telegram push** on motion  
- **Configurable** Wi-Fi & Telegram credentials  

---

## Hardware

- **Board:** ESP32 Dev Module (e.g. WROOM-32U)  
- **Sensor:** PIR motion sensor (OUT → GPIO 13)  
- **Buzzer:** Active buzzer (+ → GPIO 14, – → GND)  
- **Button:** Arm/disarm toggle (+ → 3.3 V via button, – → GPIO 26 with `INPUT_PULLUP`)  
- **Power:** USB or 5 V on VIN  

_Wiring diagram available in the project docs._

---

## Getting Started

1. **Clone the repo**  
   ```bash
   git clone https://github.com/your-username/esp32-home-alarm.git
   cd esp32-home-alarm

2. **Configure**

    // Copy this file and fill in your credentials:
    include/config_sample.h → include/config.h

3. **Install Dependencies**
      VS Code + PlatformIO IDE
      Or use PlatformIO Core (CLI)

4. **Build and Upload**

      Clean previous builds
      pio run -t clean

      Compile
      pio run

      Flash to ESP32 (make sure it’s in bootloader mode if needed)
      pio run -t upload

5. **Monitor Serial Output**
      pio device monitor --baud 115200
