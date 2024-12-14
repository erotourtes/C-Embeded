/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */


// Beep frequency
//0000 260.87 Hz C4
//0001 521.74 Hz C5
//0010 585.37 Hz D5
//0011 666.67 Hz E5
//0100 705.88 Hz F5
//0101 774.19 Hz G5
//0110 888.89 Hz A5
//0111 1000.00 Hz B5
//1000 1043.48 Hz C6
//1001 1200.00 Hz D6
//1010 1333.33 Hz E6
//1011 1411.76 Hz F6
//1100 1600.00 Hz G6
//1101 1714.29 Hz A6
//1110 2000.00 Hz B6
//1111 2181.82 Hz C7
typedef enum {
    C4 = 0x00,  // 260.87 Hz
    C5 = 0x01,  // 521.74 Hz
    D5 = 0x02,  // 585.37 Hz
    E5 = 0x03,  // 666.67 Hz
    F5 = 0x04,  // 705.88 Hz
    G5 = 0x05,  // 774.19 Hz
    A5 = 0x06,  // 888.89 Hz
    B5 = 0x07,  // 1000.00 Hz
    C6 = 0x08,  // 1043.48 Hz
    D6 = 0x09,  // 1200.00 Hz
    E6 = 0x0A,  // 1333.33 Hz
    F6 = 0x0B,  // 1411.76 Hz
    G6 = 0x0C,  // 1600.00 Hz
    A6 = 0x0D,  // 1714.29 Hz
    B6 = 0x0E,  // 2000.00 Hz
    C7 = 0x0F   // 2181.82 Hz
} SoundTone;

#define CS43L22_I2C_ADDR 0x94
#define I2C_TIMEOUT 10

uint16_t data_i2s[100] = { 0 };

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

I2S_HandleTypeDef hi2s3;
DMA_HandleTypeDef hdma_spi3_tx;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_I2C1_Init(void);
static void MX_I2S3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */





void transmit(uint8_t* buffer) {
	HAL_StatusTypeDef res = HAL_I2C_Master_Transmit(&hi2c1, CS43L22_I2C_ADDR, buffer, 2, I2C_TIMEOUT);
	if (res != HAL_OK) {
		Error_Handler();
	}
}

void CS43L22_INIT(void) {
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
	uint8_t buffer[2];

// 7.10 Playback Control 1 (Address 0Dh)
	buffer[0] = 0x0D;
	buffer[1] = 0x01;
	transmit(buffer);

//	Required Initialization Settings
//	Various sections in the device must be adjusted by implementing the initialization settings shown below after
//	power-up sequence step 3. All performance and power consumption measurements were taken with the
//	following settings:
//	1. Write 0x99 to register 0x00.
//	2. Write 0x80 to register 0x47.
//	3. Write ‘1’b to bit 7 in register 0x32.
//	4. Write ‘0’b to bit 7 in register 0x32.
//	5. Write 0x00 to register 0x00.
	buffer[0] = 0x00;
	buffer[1] = 0x99;
	transmit(buffer);

	buffer[0] = 0x47;
	buffer[1] = 0x80;
	transmit(buffer);

	buffer[0] = 0x32;
	buffer[1] = 0xFF;
	transmit(buffer);

	buffer[0] = 0x32;
	buffer[1] = 0x7F;
	transmit(buffer);

	buffer[0] = 0x00;
	buffer[1] = 0x00;
	transmit(buffer);

	buffer[0] = 0x04;
	buffer[1] = 0xAF;
	transmit(buffer);

	buffer[0] = 0x0D;
	buffer[1] = 0x00; //0x70;
	transmit(buffer);

	buffer[0] = 0x05;
	buffer[1] = 0x81;
	transmit(buffer);

	buffer[0] = 0x06;
	buffer[1] = 0x07;
	transmit(buffer);

	buffer[0] = 0x0A;
	buffer[1] = 0x00;
	transmit(buffer);

	buffer[0] = 0x27;
	buffer[1] = 0x00;
	transmit(buffer);

	buffer[0] = 0x1A;
	buffer[1] = 0x0A;
	transmit(buffer);

	buffer[0] = 0x1B;
	buffer[1] = 0x0A;
	transmit(buffer);

	buffer[0] = 0x1F;
	buffer[1] = 0x0F;
	transmit(buffer);

	buffer[0] = 0x02;
	buffer[1] = 0x9E;
	transmit(buffer);
}

void CS43L22_Beep(SoundTone pitch, uint32_t duration_ms) {
	uint8_t buffer[2];

	buffer[0] = 0x1D; // register
	buffer[1] = 0x00; // volume
	transmit(buffer);

	buffer[0] = 0x1C;
	buffer[1] = (uint8_t)pitch << 4;
	transmit(buffer);

	buffer[0] = 0x1E;
	buffer[1] = 0xC0;
	transmit(buffer);

	HAL_Delay(duration_ms);

	buffer[0] = 0x1E;
	buffer[1] = 0x00;
	transmit(buffer);
}





/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_I2C1_Init();
  MX_I2S3_Init();
  /* USER CODE BEGIN 2 */

  CS43L22_INIT();
  HAL_I2S_Transmit_DMA(&hi2s3, data_i2s, sizeof(data_i2s));

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  CS43L22_Beep(A5,300);
	  HAL_Delay(100);

	  CS43L22_Beep(D6,300);
	  HAL_Delay(100);

	  CS43L22_Beep(D6,300);
	  HAL_Delay(100);

	  CS43L22_Beep(A5,400);
	  HAL_Delay(200);

	  CS43L22_Beep(G5,300);
	  HAL_Delay(150);


	  CS43L22_Beep(A5,350);
	  HAL_Delay(100);

	  CS43L22_Beep(C6,300);
	  HAL_Delay(100);

	  CS43L22_Beep(E6,350);
	  HAL_Delay(150);

	  CS43L22_Beep(D6,400);
	  HAL_Delay(200);


	  CS43L22_Beep(B5,300);
	  HAL_Delay(100);

	  CS43L22_Beep(A5,400);
	  HAL_Delay(250);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Macro to configure the PLL multiplication factor
  */
  __HAL_RCC_PLL_PLLM_CONFIG(16);

  /** Macro to configure the PLL clock source
  */
  __HAL_RCC_PLL_PLLSOURCE_CONFIG(RCC_PLLSOURCE_HSI);

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief I2S3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2S3_Init(void)
{

  /* USER CODE BEGIN I2S3_Init 0 */

  /* USER CODE END I2S3_Init 0 */

  /* USER CODE BEGIN I2S3_Init 1 */

  /* USER CODE END I2S3_Init 1 */
  hi2s3.Instance = SPI3;
  hi2s3.Init.Mode = I2S_MODE_MASTER_TX;
  hi2s3.Init.Standard = I2S_STANDARD_PHILIPS;
  hi2s3.Init.DataFormat = I2S_DATAFORMAT_16B;
  hi2s3.Init.MCLKOutput = I2S_MCLKOUTPUT_ENABLE;
  hi2s3.Init.AudioFreq = I2S_AUDIOFREQ_48K;
  hi2s3.Init.CPOL = I2S_CPOL_LOW;
  hi2s3.Init.ClockSource = I2S_CLOCK_PLL;
  hi2s3.Init.FullDuplexMode = I2S_FULLDUPLEXMODE_DISABLE;
  if (HAL_I2S_Init(&hi2s3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2S3_Init 2 */

  /* USER CODE END I2S3_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin : PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
