# STM32 IoT Development: B-L4S5I-IOT01A Discovery Kit
![alt text](image.png)

This repository serves as a centralized hub for firmware development, sensor interfacing, and IoT protocols using the **STM32B-L4S5I-IOT01A** Discovery board[cite: 2, 3].

## 📖 Overview
The goal of this repo is to document the implementation of various embedded systems projects, ranging from low-level peripheral drivers (I2C, SPI, UART) to wireless communication (BLE, Wi-Fi)[cite: 4, 18, 20]. 

## 🛠 Hardware Specifications
The projects are built on the **STM32L4S5VI** microcontroller[cite: 6, 13]:
* **Core:** Arm® Cortex®-M4 with FPU and DSP instructions[cite: 7].
* **Clock Speed:** Up to 120MHz[cite: 8].
* **Memory:** 2MB Internal Flash and 640KB SRAM[cite: 9, 10].
* **Accelerators:** ART Accelerator, Chrom-ART (Graphics), and hardware AES/HASH encryption[cite: 11].

## 📡 Integrated Sensors & Connectivity
The board is equipped as a self-contained IMU and environmental station[cite: 32]:
* **Motion:** 3D Accelerometer and 3D Gyroscope (LSM6DSL) + 3-axis Magnetometer (LIS3MDL)[cite: 33].
* **Environment:** Humidity/Temperature (HTS221) and Barometer (LPS22HB)[cite: 34].
* **Distance:** Time-of-Flight (ToF) sensor (VL53L0X)[cite: 35].
* **Wireless:** On-board modules for Wi-Fi (802.11 b/g/n), Bluetooth® V4.1, and NFC[cite: 20, 21].

## 🚀 Project List


## 📂 Documentation
Technical references used in this repository:
* [Board Introduction PDF](./Docs/stm32l4s5vi_intro.pdf)
* [Full Hardware Specifications PDF](Project1_Understanding_STM32Boards/stm32l4s5vi.pdf)

---
**Author:** Dhiraj  
