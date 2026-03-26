/**
  ******************************************************************************
  * @file    Wifi/WiFi_Client_Server/src/main.c
  * @author  MCD Application Team
  * @brief   This file provides main program functions
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
/* Private defines -----------------------------------------------------------*/

#define TERMINAL_USE

/* Update SSID and PASSWORD with own Access point settings */
#define SSID     "Room-303"
#define PASSWORD "303@spit"

uint8_t RemoteIP[] = {0,0,0,0};// This is the IP address you will communicate with
#define RemotePORT	8002

#define WIFI_WRITE_TIMEOUT 10000
#define WIFI_READ_TIMEOUT  10000

#define CONNECTION_TRIAL_MAX          10

#if defined (TERMINAL_USE)
#define TERMOUT(...)  printf(__VA_ARGS__)
#else
#define TERMOUT(...)
#endif

/* Private variables ---------------------------------------------------------*/
#if defined (TERMINAL_USE)
extern UART_HandleTypeDef hDiscoUart;
#endif /* TERMINAL_USE */
static uint8_t RxData [500];


/* Private function prototypes -----------------------------------------------*/
#if defined (TERMINAL_USE)
#ifdef __GNUC__
/* With GCC, small TERMOUT (option LD Linker->Libraries->Small TERMOUT
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
#endif /* TERMINAL_USE */

static void SystemClock_Config(void);
void Convert_String_To_IP(const char* ipString, uint8_t* ipArray);
extern  SPI_HandleTypeDef hspi;
/* Private functions ---------------------------------------------------------*/

void Convert_String_To_IP(const char* ipString, uint8_t* ipArray)
{
    unsigned int ip[4] = {0};
    /* sscanf reads the string and splits it at the periods */
    if (sscanf(ipString, "%u.%u.%u.%u", &ip[0], &ip[1], &ip[2], &ip[3]) == 4)
    {
        ipArray[0] = (uint8_t)ip[0];
        ipArray[1] = (uint8_t)ip[1];
        ipArray[2] = (uint8_t)ip[2];
        ipArray[3] = (uint8_t)ip[3];
    }
}
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  uint8_t  MAC_Addr[6] = {0};
  uint8_t  IP_Addr[4] = {0};
  //uint8_t TxData[] = "STM32 : Hello!\n";
  int32_t Socket = -1;
  uint16_t Datalen;
  int32_t ret;
  int16_t Trials = CONNECTION_TRIAL_MAX;

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* Configure the system clock */
  SystemClock_Config();
  /* Configure LED2 */

  BSP_LED_Init(LED2);

#if defined (TERMINAL_USE)
  /* Initialize all configured peripherals */
  hDiscoUart.Instance = DISCOVERY_COM1;
  hDiscoUart.Init.BaudRate = 115200;
  hDiscoUart.Init.WordLength = UART_WORDLENGTH_8B;
  hDiscoUart.Init.StopBits = UART_STOPBITS_1;
  hDiscoUart.Init.Parity = UART_PARITY_NONE;
  hDiscoUart.Init.Mode = UART_MODE_TX_RX;
  hDiscoUart.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  hDiscoUart.Init.OverSampling = UART_OVERSAMPLING_16;
  hDiscoUart.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  hDiscoUart.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

  BSP_COM_Init(COM1, &hDiscoUart);
#endif /* TERMINAL_USE */

  TERMOUT("====== WIFI Module in TCP Client mode demonstration ===== \n\r");
  TERMOUT("TCP Client Instructions :\n\r");
  TERMOUT("1. Make sure your board is connected to the same access point as your PC (update the code)\n\r" );
  TERMOUT("2. Make sure the RemoteIP matches the IPAddress of your PC (the device you're going to connect to\n\r");
  TERMOUT("3. Create a server using Python over the PC to listen to the port 8002.\n\r");
  TERMOUT("4. The board will automatically try to connect to the RemoteIP and standby for your command\n\r");


  /*Initialize  WIFI module */
  if(WIFI_Init() ==  WIFI_STATUS_OK)
  {
    TERMOUT("> WIFI Module Initialized.\n\r");
    if(WIFI_GetMAC_Address(MAC_Addr, sizeof(MAC_Addr)) == WIFI_STATUS_OK)
    {
      TERMOUT("> es-wifi module MAC Address : %X:%X:%X:%X:%X:%X\n\r",
               MAC_Addr[0],
               MAC_Addr[1],
               MAC_Addr[2],
               MAC_Addr[3],
               MAC_Addr[4],
               MAC_Addr[5]);
    }
    else
    {
      TERMOUT("> ERROR : CANNOT get MAC address\n\r");
      BSP_LED_On(LED2);
    }

    if( WIFI_Connect(SSID, PASSWORD, WIFI_ECN_WPA2_PSK) == WIFI_STATUS_OK)
    {
      TERMOUT("> es-wifi module connected \n\r");
      if(WIFI_GetIP_Address(IP_Addr, sizeof(IP_Addr)) == WIFI_STATUS_OK)
      {
        TERMOUT("> es-wifi module got IP Address : %d.%d.%d.%d\n\r",
               IP_Addr[0],
               IP_Addr[1],
               IP_Addr[2],
               IP_Addr[3]);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET);

        /* --- INTERACTIVE IP INPUT LOOP --- */
        TERMOUT("\n\rType Destination IP (e.g. 172.20.10.9) and press Enter: ");
        fflush(stdout);
                char ipInput[16] = {0};
                uint8_t rxChar = 0;
                uint8_t idx = 0;
                /* Listen to PuTTY character by character */
                while (idx < 15)
                {
                  /* Wait indefinitely for a single key press */
                  if (HAL_UART_Receive(&hDiscoUart, &rxChar, 1, HAL_MAX_DELAY) == HAL_OK)
                  {
                    TERMOUT("%c", rxChar); /* Echo the character back to PuTTY */

                    /* Stop listening if Enter (\r or \n) is pressed */
                    if (rxChar == '\r' || rxChar == '\n')
                    {
                      ipInput[idx] = '\0'; /* Properly terminate the C string */
                      break;
                    }

                    ipInput[idx] = rxChar;
                    idx++;
                  }
                }
                TERMOUT("\n\r");

                /* Convert your typed string into the RemoteIP byte array */
                Convert_String_To_IP(ipInput, RemoteIP);
                /* --------------------------------- */

                TERMOUT("> Trying to connect to Server: %d.%d.%d.%d:%d ...\n\r",
                       RemoteIP[0], RemoteIP[1], RemoteIP[2], RemoteIP[3], RemotePORT);

        while (Trials--)
        {
          if( WIFI_OpenClientConnection(0, WIFI_TCP_PROTOCOL, "TCP_CLIENT", RemoteIP, RemotePORT, 0) == WIFI_STATUS_OK)
          {
            TERMOUT("> TCP Connection opened successfully.\n\r");
            Socket = 0;
            break;
          }
        }
        if(Socket == -1)
        {
          TERMOUT("> ERROR : Cannot open Connection\n\r");
          BSP_LED_On(LED2);
          HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
        }
      }
      else
      {
        TERMOUT("> ERROR : es-wifi module CANNOT get IP address\n\r");
        BSP_LED_On(LED2);
        HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
      }
    }
    else
    {
      TERMOUT("> ERROR : es-wifi module NOT connected\n\r");
      BSP_LED_On(LED2);
      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
    }
  }
  else
  {
    TERMOUT("> ERROR : WIFI Module cannot be initialized.\n\r");
    BSP_LED_On(LED2);
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
  }

  while(1)
  {
	  char termRxBuffer[128] = {0}; /* Buffer to hold what you type */
	    uint8_t termRxIndex = 0;
	    uint8_t uartChar = 0;

	    TERMOUT("\n\rType a message and press Enter to send to Python:\n\r> ");
	    fflush(stdout);

	    while(1)
	    {
	      if(Socket != -1)
	      {
	        /* 1. FAST LISTEN: Check Wi-Fi for incoming Python messages (50ms timeout) */
	        ret = WIFI_ReceiveData(Socket, RxData, sizeof(RxData)-1, &Datalen, 50);

	        if(ret == WIFI_STATUS_OK && Datalen > 0)
	        {
	          RxData[Datalen] = '\0';
	          TERMOUT("\n\r[SERVER]: %s", RxData);
	          TERMOUT("\n\r> "); /* Reprint the prompt marker */
	          fflush(stdout);
	        }

	        /* 2. FAST LISTEN: Check PuTTY for your typing (0ms timeout) */
	        if (HAL_UART_Receive(&hDiscoUart, &uartChar, 1, 0) == HAL_OK)
	              {
	                /* Echo the character so you can see what you type */
	                TERMOUT("%c", uartChar);
	                fflush(stdout);

	                /* If you press Enter, send the message! */
	                if (uartChar == '\r' || uartChar == '\n')
	                {
	                  TERMOUT("\n\r"); /* <-- NEW: Visually drop the cursor to the next line in PuTTY */

	                  if (termRxIndex > 0) /* Don't send empty strings */
	                  {
	                    /* NEW: Explicitly append newline characters so Python knows the message is done */
	                    termRxBuffer[termRxIndex] = '\r';
	                    termRxBuffer[termRxIndex + 1] = '\n';
	                    termRxBuffer[termRxIndex + 2] = '\0';

	                    uint16_t sendLen;
	                    /* Send your typed string over Wi-Fi */
	                    ret = WIFI_SendData(Socket, (uint8_t*)termRxBuffer, strlen(termRxBuffer), &sendLen, WIFI_WRITE_TIMEOUT);

	                    if (ret != WIFI_STATUS_OK)
	                    {
	                      TERMOUT("\n\r> ERROR : Failed to Send Data\n\r");
	                      HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET);
	                      break; /* Disconnect */
	                    }
	                  }

	                  /* Reset the typing buffer for your next message */
	                  termRxIndex = 0;
	                  TERMOUT("> "); /* Print the fresh prompt for the next line */
	                  fflush(stdout);
	                }
	                /* Handle Backspace key (PuTTY usually sends \b or 0x7F) */
	                else if (uartChar == '\b' || uartChar == 0x7F)
	                {
	                  if (termRxIndex > 0)
	                  {
	                    termRxIndex--;
	                    TERMOUT(" \b"); /* Visually erase the character in PuTTY */
	                    fflush(stdout);
	                  }
	                }
	                /* Normal characters: add to the buffer */
	                else
	                {
	                  /* Leave room at the end of the buffer for the \r\n\0 we add later */
	                  if (termRxIndex < sizeof(termRxBuffer) - 3)
	                  {
	                    termRxBuffer[termRxIndex++] = uartChar;
	                  }
	                }
	              }
	      }
	    }
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow :
  *            System Clock source            = PLL (MSI)
  *            SYSCLK(Hz)                     = 80000000
  *            HCLK(Hz)                       = 80000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 1
  *            APB2 Prescaler                 = 1
  *            MSI Frequency(Hz)              = 4000000
  *            PLL_M                          = 1
  *            PLL_N                          = 40
  *            PLL_R                          = 2
  *            PLL_P                          = 7
  *            PLL_Q                          = 4
  *            Flash Latency(WS)              = 4
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 40;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    /* Initialization Error */
    while(1);
  }
}

#if defined (TERMINAL_USE)
/**
  * @brief  Retargets the C library TERMOUT function to the USART.
  * @param  None
  * @retval None
  */
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
  HAL_UART_Transmit(&hDiscoUart, (uint8_t *)&ch, 1, 0xFFFF);

  return ch;
}
#endif /* TERMINAL_USE */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: TERMOUT("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* 1. Print a critical failure message to PuTTY */
#if defined (TERMINAL_USE)
  TERMOUT("\r\n[!] FATAL ERROR: System halted in Error_Handler()\r\n");
#endif

  /* 2. Disable all interrupts to freeze the system state */
  __disable_irq();

  /* 3. Stay in this loop forever and blink the LED rapidly */
  while (1)
  {
    BSP_LED_Toggle(LED2);

    /* Simple delay loop since HAL_Delay might not work with interrupts disabled */
    for (volatile uint32_t i = 0; i < 1000000; i++) { }
  }
}
/**
  * @brief  EXTI line detection callback.
  * @param  GPIO_Pin: Specifies the port pin connected to corresponding EXTI line.
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
  switch (GPIO_Pin)
  {
    case (GPIO_PIN_1):
    {
      SPI_WIFI_ISR();
      break;
    }
    default:
    {
      break;
    }
  }
}

void SPI3_IRQHandler(void)
{
  HAL_SPI_IRQHandler(&hspi);
}

/**
  * @brief This function handles System tick timer.
  */

/**
  * @brief This function handles EXTI line 1 interrupt.
  */
