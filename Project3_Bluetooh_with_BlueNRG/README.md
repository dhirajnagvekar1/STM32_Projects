# Interfacing BlueNRG with STM32

This project demonstrates how to interface the **ST BlueNRG-MS** (or BlueNRG-2) Bluetooth Low Energy (BLE) network processor with an **BLS4S5I ** microcontroller using the SPI protocol.

---

## 🛠 Prerequisites

* **Hardware:** * STM32 MCU (e.g., Nucleo-L476RG or F401RE).
    * BlueNRG expansion board (e.g., [X-NUCLEO-IDB05A2](https://www.st.com/en/ecosystems/x-nucleo-idb05a2.html)).
* **Software:** * STM32CubeIDE.
    * X-CUBE-BLE1 expansion pack (Standard HCI layer).

---

## 🔌 Hardware Configuration (SPI)

The BlueNRG module acts as a SPI slave. Ensure the following pins are mapped correctly in your **.ioc** file:

| BlueNRG Pin | STM32 Peripheral | Role |
| :--- | :--- | :--- |
| **MOSI** | SPI Master Out | Data to BlueNRG |
| **MISO** | SPI Master In | Data from BlueNRG |# Interfacing BlueNRG with STM32

This project demonstrates how to interface the **ST BlueNRG-MS** (or BlueNRG-2) Bluetooth Low Energy (BLE) network processor with an **STM32** microcontroller using the SPI protocol.

---

## 🛠 Prerequisites

* **Hardware:** * STM32 MCU (e.g., Nucleo-L476RG or F401RE).
    * BlueNRG expansion board (e.g., [X-NUCLEO-IDB05A2](https://www.st.com/en/ecosystems/x-nucleo-idb05a2.html)).
* **Software:** * STM32CubeIDE.
    * X-CUBE-BLE1 expansion pack (Standard HCI layer).

---

## 🔌 Hardware Configuration (SPI)

The BlueNRG module acts as a SPI slave. Ensure the following pins are mapped correctly in your **.ioc** file:

| BlueNRG Pin | STM32 Peripheral | Role |
| :--- | :--- | :--- |
| **MOSI** | SPI Master Out | Data to BlueNRG |
| **MISO** | SPI Master In | Data from BlueNRG |
| **SCK** | SPI Clock | Serial Clock |
| **CSN** | GPIO Output | SPI Chip Select (Active Low) |
| **IRQ** | GPIO_EXTI | Interrupt from BlueNRG (Data Ready) |
| **RESET** | GPIO Output | Hardware Reset |

### SPI Parameters
* **Mode:** Full-Duplex Master
* **Data Size:** 8 Bits
* **First Bit:** MSB First
* **CPOL:** Low (0)
* **CPHA:** 1 Edge (0)

---

## 🚀 Software Implementation

### 1. Initialization
The BlueNRG requires a specific boot sequence. You must initialize the HCI (Host Controller Interface) and then the GATT/GAP layers.

```c
/* USER CODE BEGIN 2 */
// 1. Initialize the BlueNRG hardware interface
hci_init(user_notify, NULL);

// 2. Reset BlueNRG-MS
BlueNRG_RST();

// 3. Initialize BLE Stack
aci_gatt_init();
aci_gap_init(GAP_PERIPHERAL_ROLE, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);

// 4. Set Device Name
aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0, 7, (uint8_t*)"STM32");
/* USER CODE END 2 */# Interfacing BlueNRG with STM32

This project demonstrates how to interface the **ST BlueNRG-MS** (or BlueNRG-2) Bluetooth Low Energy (BLE) network processor with an **STM32** microcontroller using the SPI protocol.

---

## 🛠 Prerequisites

* **Hardware:** * STM32 MCU (e.g., Nucleo-L476RG or F401RE).
    * BlueNRG expansion board (e.g., [X-NUCLEO-IDB05A2](https://www.st.com/en/ecosystems/x-nucleo-idb05a2.html)).
* **Software:** * STM32CubeIDE.
    * X-CUBE-BLE1 expansion pack (Standard HCI layer).

---

## 🔌 Hardware Configuration (SPI)

The BlueNRG module acts as a SPI slave. Ensure the following pins are mapped correctly in your **.ioc** file:

| BlueNRG Pin | STM32 Peripheral | Role |
| :--- | :--- | :--- |
| **MOSI** | SPI Master Out | Data to BlueNRG |
| **MISO** | SPI Master In | Data from BlueNRG |
| **SCK** | SPI Clock | Serial Clock |
| **CSN** | GPIO Output | SPI Chip Select (Active Low) |
| **IRQ** | GPIO_EXTI | Interrupt from BlueNRG (Data Ready) |
| **RESET** | GPIO Output | Hardware Reset |

### SPI Parameters
* **Mode:** Full-Duplex Master
* **Data Size:** 8 Bits
* **First Bit:** MSB First
* **CPOL:** Low (0)
* **CPHA:** 1 Edge (0)

---

## 🚀 Software Implementation

### 1. Initialization
The BlueNRG requires a specific boot sequence. You must initialize the HCI (Host Controller Interface) and then the GATT/GAP layers.

```c
/* USER CODE BEGIN 2 */
// 1. Initialize the BlueNRG hardware interface
hci_init(user_notify, NULL);

// 2. Reset BlueNRG-MS
BlueNRG_RST();

// 3. Initialize BLE Stack
aci_gatt_init();
aci_gap_init(GAP_PERIPHERAL_ROLE, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);

// 4. Set Device Name
aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0, 7, (uint8_t*)"STM32");
/* USER CODE END 2 */
| **SCK** | SPI Clock | Serial Clock |
| **CSN** | GPIO Output | SPI Chip Select (Active Low) |
| **IRQ** | GPIO_EXTI | Interrupt from BlueNRG (Data Ready) |
| **RESET** | GPIO Output | Hardware Reset |

### SPI Parameters
* **Mode:** Full-Duplex Master
* **Data Size:** 8 Bits
* **First Bit:** MSB First
* **CPOL:** Low (0)
* **CPHA:** 1 Edge (0)

---

## 🚀 Software Implementation

### 1. Initialization
The BlueNRG requires a specific boot sequence. You must initialize the HCI (Host Controller Interface) and then the GATT/GAP layers.

```c
/* USER CODE BEGIN 2 */
// 1. Initialize the BlueNRG hardware interface
hci_init(user_notify, NULL);

// 2. Reset BlueNRG-MS
BlueNRG_RST();

// 3. Initialize BLE Stack
aci_gatt_init();
aci_gap_init(GAP_PERIPHERAL_ROLE, 0, 0x07, &service_handle, &dev_name_char_handle, &appearance_char_handle);

// 4. Set Device Name
aci_gatt_update_char_value(service_handle, dev_name_char_handle, 0, 7, (uint8_t*)"STM32");
/* USER CODE END 2 */