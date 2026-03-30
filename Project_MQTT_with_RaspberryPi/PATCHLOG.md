## 🛠 Technical Patch Log
During the development of this project, several critical modifications were made to the standard ST Middleware and HAL drivers to ensure stability on the B-L4S5I-IOT01A board.

### 1. Middleware Fixes (`es_wifi.c`)
- **Variable Declaration:** Added `char *ptr;` inside the `if(strstr... "Accepted")` block to resolve an "undeclared identifier" error during the IP parsing stage.
- **Function Signature Update:** Updated the `ParseIP` call to include the buffer size argument: `ParseIP((char *)ptr, conn->RemoteIP, 4);`.
- **Macro Conflict Resolution:** Added an `#ifdef DEBUG #define DEBUG 1 #endif` block before the ST driver's debug definitions to stop "redefined" compiler errors caused by global IDE flags.

### 2. Hardware & Interrupt Layer (`stm32l4xx_it.c`)
- **Interrupt Vector Alignment:** Manually added `EXTI1_IRQHandler` and `SPI3_IRQHandler` to the interrupt table.
- **Data-Ready Synchronization:** Configured `HAL_GPIO_EXTI_Callback` to trigger `WIFIIO_AppDataReady_ISR()`. This allowed the MCU to "hear" the Inventek Wi-Fi module, resolving the "System Freeze" during the `WIFI_Connect` phase.
- **Handle Mapping:** Updated the SPI handle in the ISR by adding the extern SPI_HandleTypeDef hspi; which is defined in es_wifi_io.c

### 3. Paho MQTT Library Optimization
- **Binary Footprint Reduction:** Excluded `MQTTClient.c/h` to remove the `Unknown type name 'Network'` dependency.
- **Manual Serialization:** Shifted to the **Packet Layer** (`MQTTPacket.c`, `MQTTConnectClient.c`) to handle raw socket transmission, significantly reducing RAM usage.

