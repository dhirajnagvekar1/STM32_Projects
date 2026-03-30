# STM32 to Raspberry Pi Telemetry System using MQTT

A high-performance IoT implementation using the **ARM Cortex-M4 (STM32L4S5)** to stream real-time environmental data to a local **Raspberry Pi MQTT Broker**. This project demonstrates low-level driver patching, interrupt-driven Wi-Fi communication, and JSON data serialization.


## 📍 Project Overview
This project implements a localized **Industrial IoT (IIoT)** telemetry node using the **STM32 B-L4S5I-IOT01A** Discovery kit. The system transitions away from high-latency cloud platforms toward a high-speed, private **Edge Computing** architecture.

By interfacing an onboard **LPS22HB/BMP280** sensor suite with a **Raspberry Pi MQTT Broker**, the node provides real-time environmental monitoring over a local Wi-Fi mesh. The core of this project involved custom firmware engineering to bypass middleware limitations, including manual MQTT packet serialization and low-level interrupt synchronization between the MCU and the Inventek Wi-Fi module.


## 🛠️ Tech Stack
- **Hardware:** STM32 B-L4S5I-IOT01A Discovery Kit, Raspberry Pi 4.
- **Sensors:** HTS221 / LPS22HB (Temperature & Pressure).
- **Communication:** MQTT (Paho Packet Level), SPI3, Wi-Fi (802.11 b/g/n).
- **Languages:** Embedded C (HAL), Python 3 (Subscriber).
- **Tools:** STM32CubeIDE, MQTT Explorer, Mosquitto Broker.

---

## 🚀 Key Features & Technical Solutions

### 1. Ⓜ️Manual MQTT Serialization
Instead of the heavy Paho Client library, this project utilizes the **MQTTPacket layer**. This reduces memory footprint and allows for manual socket management over the `es_wifi` interface.

### 2. ⁉️ Custom Interrupt Logic (The "Freeze" Fix)
Implemented dedicated Interrupt Service Routines (ISRs) to handle the asynchronous "Data Ready" signals from the Wi-Fi module:
- **EXTI1:** Handles the PE1 hardware trigger.
- **SPI3:** Manages high-speed data transfer between the MCU and the Inventek module.

### 3. 🪛 Driver Patches
Resolved several critical vendor-specific bugs:
- Fixed **IP Parsing** logic in `es_wifi.c` by adding missing pointer declarations.
- Resolved **Macro Conflicts** by undefining global `DEBUG` flags that interfered with ST drivers.
- Corrected **SPI Handle** naming from generic `hspi` to hardware-specific `hspi3`.

### 4. 🌐 Potential Applications
- **Industrial Condition Monitoring:** Real-time vibration and thermal tracking in factory environments without cloud dependency.
- **Secure Lab Environments:** Air-gapped telemetry for sensitive research facilities requiring local data sovereignty.
- **Smart Agriculture:** Localized sensor meshes for greenhouses where external internet connectivity is unreliable.
- **Edge Analytics:** Rapid prototyping for systems requiring sub-millisecond response times for sensor-triggered actions.

## 🛠 Technical Patch Log
During the development of this project, several critical modifications were made to the standard ST Middleware and HAL drivers to ensure stability. 

Detailed documentation of these changes—including the SPI3 handle fix, `DEBUG` macro conflict resolution, and EXTI synchronization—can be found here:

👉 **[View the Full Patch Log](./PATCHLOG.md)**

## 📂 Repository Structure
```text
├───Dhiraj_MQTT
│   ├───Core
│   │   ├───Inc
│   │   ├───Src
│   │   └───Startup
│   └───Drivers
│       ├───BSP
│       │   ├───B-L4S5I-IOT01
│       │   └───Components
│       ├───CMSIS
│       │   ├───Device
│       │   │   └───ST
│       │   │       └───STM32L4xx
│       │   │           ├───Include
│       │   │           └───Source
│       │   │               └───Templates
│       │   └───Include
│       └───STM32L4xx_HAL_Driver
│           ├───Inc
│           │   └───Legacy
│           └───Src
└───Scripts
    ├───pc
    │       mqtt_client.py
    │
    └───raspberry
            client_sub.py

